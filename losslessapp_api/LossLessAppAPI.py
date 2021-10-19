import os
from flask import Flask, render_template, request, jsonify
from flask_restful import Api, Resource, reqparse, abort, fields, marshal_with
from flask_sqlalchemy import SQLAlchemy
import uuid
from werkzeug.security import generate_password_hash, check_password_hash
import jwt
import datetime
from functools import wraps

key_path = os.path.abspath(os.path.join(os.path.abspath(__file__), '..' + '/secret_key.txt'))
try:
    f = open(key_path, 'r')
except FileNotFoundError:
    print("Secret Key file is missing!")
    exit(-1)

app = Flask(__name__)
api = Api(app)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///cryptocurrencies.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['SECRET_KEY'] = f.read()
f.close()

@app.route('/')
def index():
    return render_template('index.html')

db = SQLAlchemy(app)

class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    public_id = db.Column(db.String(50), unique=True)
    username = db.Column(db.String(50))
    password = db.Column(db.String(80))
    balance = db.Column(db.Float)
    admin = db.Column(db.Boolean)

    def __repr__(self):
        return f"Public_id: {User.public_id}, username: {User.username}, balance: {User.balance}, is admin: {User.admin}"

class CryptoModel(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False)
    value = db.Column(db.Float, nullable=False)

    def __repr__(self):
        return f"ID: {CryptoModel.id}, name: {CryptoModel.name}, value: {CryptoModel.value}"

db.create_all()
def token_required(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = None

        if 'x-access-token' in request.headers:
            token = request.headers['x-access-token']

        if not token:
            abort(401, message="Missing authorization token! Please Sing In or Sign Up!")
        
        try:
            data = jwt.decode(token, app.config['SECRET_KEY'], algorithms=["HS256"])
            current_user = User.query.filter_by(public_id=data['public_id']).first()
        except:
            abort(401, message="Token is invalid!")
        
        return f(current_user, *args, **kwargs)
    return decorated

# Create request parser for user sign up
signup_post_args = reqparse.RequestParser()
signup_post_args.add_argument("username", type=str)
signup_post_args.add_argument("password", type=str)
signup_post_args.add_argument("balance", type=float)

# Create request parser for user sign in
signin_post_args = reqparse.RequestParser()
signin_post_args.add_argument("username", type=str)
signin_post_args.add_argument("password", type=str)

# Create post arguments, where all fields are required
crypto_post_args = reqparse.RequestParser()
crypto_post_args.add_argument("name", type=str, required=True)
crypto_post_args.add_argument("value", type=float, required=True)

# In patch we can modify any of value, but we need to give an ID of the record
crypto_patch_args = reqparse.RequestParser()
crypto_patch_args.add_argument("name", type=str)
crypto_patch_args.add_argument("value", type=float)

serializing_fields_crypto = {
    'id': fields.Integer,
    'name': fields.String,
    'value': fields.Float
}

class SignUp(Resource):
    @token_required
    def get(current_user, self):
        if not current_user:
            abort(404, message="No user found!")
        if not current_user.admin:
            abort(401, message="Access denied!")
        users = User.query.all()

        output = []
        for user in users:
            single_user = {}
            single_user['id'] = user.id
            single_user['public_id'] = user.public_id
            single_user['username'] = user.username
            single_user['password'] = user.password
            single_user['balance'] = user.balance
            single_user['admin'] = user.admin
            output.append(single_user)
        return jsonify(output)

    def post(self):
        args = signup_post_args.parse_args()
        if not args or not args['username'] or not args['password'] or not args['balance']:
            abort(400, message="Missing one or more fields!")
        result = User.query.filter_by(username=args['username']).first()
        if result:
            abort(409, message="Username is already taken!")
        hashed_password = generate_password_hash(args['password'], method='sha256')

        new_user = User(
            public_id=str(uuid.uuid4()), 
            username=args['username'], 
            password=hashed_password,
            balance=args['balance'],
            admin=False)
        db.session.add(new_user)
        db.session.commit()
        return {"message": "Succesully added new user."}, 201


class SignIn(Resource):
    def post(self):
        args = signin_post_args.parse_args()
        if not args['username'] or not args['password']:
            abort(401, message="Missing username or login!")
        
        user = User.query.filter_by(username=args['username']).first()

        if not user:
            abort(401, message="Wrong username/password or user does not exist!")
        
        if check_password_hash(user.password, args['password']):
            token = jwt.encode({'public_id': user.public_id, 'exp': datetime.datetime.utcnow() + datetime.timedelta(minutes=30)},app.config['SECRET_KEY'],algorithm="HS256")
            return {"token": token}, 200
        
        # We get there only when password is wrong, but for safety issues we cannot say that to an user, so we display same result as when wrong username.
        abort(401, message="Wrong username or password user does not exist!")

class Cryptocurrency(Resource):
    # GET result by id
    @token_required
    @marshal_with(serializing_fields_crypto)
    def get(current_user, self, crypto_id):
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if not result:
            abort(404, message="Crypto with that ID doesn't exist!")
        return result
    
    @token_required
    @marshal_with(serializing_fields_crypto)
    def post(current_user, self, crypto_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = crypto_post_args.parse_args()
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if result:
            abort(409, message="Crypto ID is already taken!")
        crypto = CryptoModel(id=crypto_id, name=args['name'], value=args['value'])
        db.session.add(crypto)
        db.session.commit()
        return crypto, 201

    @token_required
    # Attach token with no expiration date to updating script!
    @marshal_with(serializing_fields_crypto)
    def patch(current_user, self, crypto_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = crypto_patch_args.parse_args()
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if not result:
            abort(404, message="Crypto with that ID doesn't exist!")
        
        if args['name']:
            result.name = args['name']
        if args['value']:
            result.value = args['value']
        
        db.session.commit()
        return result, 200

    @token_required
    def delete(current_user, self, crypto_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if not result:
            abort(404, message="Crypto with that ID doesn't exist!")
        db.session.delete(result)
        db.session.commit()
        return {"message": "Succesfully removed record from database."}, 200

class Cryptocurrencies(Resource):
    # GET all the cryptocurrencies
    @token_required
    @marshal_with(serializing_fields_crypto)
    def get(current_user, self):
        result = CryptoModel.query.all()
        if not result:
            abort(404, message="There are no cryptocurrencies in the database!")
        return result
    
    # DELETE whole table
    # IMPLEMENTED JUST FOR TESTING, REMOVE ON PRODUCTION!
    @token_required
    def delete(current_user, self):
        if not current_user.admin:
            abort(401, message="Access denied!")
        result = CryptoModel.query.all()
        if not result:
            abort(404, message="There are no cryptocurrencies in the database!")
        CryptoModel.query.delete()
        db.session.commit()
        return {"message": "Succesfully cleared database."}, 200


# Endpoint to access single cryptocurrency by ID
api.add_resource(Cryptocurrency, "/cryptocurrencies/<int:crypto_id>")
# Endpoint for access to all cryptocurrencies
api.add_resource(Cryptocurrencies, "/cryptocurrencies")
# Endpoint for sign up 
api.add_resource(SignUp, "/register")
# Endpoint for sing in
api.add_resource(SignIn, "/login")

if __name__ == "__main__":
    app.run(debug=True)