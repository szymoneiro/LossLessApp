import os
from flask import Flask, render_template
from flask_restful import Api, Resource, reqparse, abort, fields, marshal_with
from flask_sqlalchemy import SQLAlchemy

# Obtain SECRET_KEY
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

# Create single cryptocurrency model in database
class CryptoModel(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False)
    value = db.Column(db.Float, nullable=False)

    def __repr__(self):
        return f"ID: {CryptoModel.id}, name: {CryptoModel.name}, value: {CryptoModel.value}"

db.create_all()

# Create put arguments, where all fields are required
crypto_put_args = reqparse.RequestParser()
crypto_put_args.add_argument("name", type=str, help="Name of the cryptocurrency", required=True)
crypto_put_args.add_argument("value", type=float, help="Value of the cryptocurrency", required=True)

# In patch we can modify any of value, but we need to give an ID of the record
crypto_patch_args = reqparse.RequestParser()
crypto_patch_args.add_argument("name", type=str)
crypto_patch_args.add_argument("value", type=float)

serializing_fields = {
    'id': fields.Integer,
    'name': fields.String,
    'value': fields.Float
}

class Cryptocurrency(Resource):
    @marshal_with(serializing_fields)
    # GET result by id
    def get(self, crypto_id):
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if not result:
            abort(404, message="Crypto with that ID doesn't exist!")
        return result
    
    @marshal_with(serializing_fields)
    def put(self, crypto_id):
        args = crypto_put_args.parse_args()
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if result:
            abort(409, message="Crypto ID is already taken!")
        crypto = CryptoModel(id=crypto_id, name=args['name'], value=args['value'])
        db.session.add(crypto)
        db.session.commit()
        return crypto, 201

    @marshal_with(serializing_fields)
    def patch(self, crypto_id):
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

    def delete(self, crypto_id):
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if not result:
            abort(404, message="Crypto with that ID doesn't exist!")
        db.session.delete(result)
        db.session.commit()
        return {"message": "Succesfully removed record from database."}, 200

class Cryptocurrencies(Resource):
    @marshal_with(serializing_fields)
    # GET all the cryptocurrencies
    def get(self):
        result = CryptoModel.query.all()
        if not result:
            abort(404, message="There are no cryptocurrencies in the database!")
        return result
    
    # DELETE whole table
    # IMPLEMENTED JUST FOR TESTING, REMOVE ON PRODUCTION!
    def delete(self):
        result = CryptoModel.query.all()
        if not result:
            abort(404, message="There are no cryptocurrencies in the database!")
        CryptoModel.query.delete()
        db.session.commit()
        return {"message": "Succesfully cleared database."}, 200


# Endpoint to access cryptocurrencies by name
api.add_resource(Cryptocurrency, "/cryptocurrencies/<int:crypto_id>")
api.add_resource(Cryptocurrencies, "/cryptocurrencies")

if __name__ == "__main__":
    app.run(debug=True)