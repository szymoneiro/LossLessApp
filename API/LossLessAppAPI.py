import os
from flask import Flask, render_template, request, jsonify
from flask_restful import Api, Resource, reqparse, abort, fields, marshal_with
from flask_sqlalchemy import SQLAlchemy
import uuid
from sqlalchemy.orm import backref
from werkzeug.security import generate_password_hash, check_password_hash
import jwt
import datetime
from functools import wraps
import datetime
import enum

key_path = os.path.abspath(os.path.join(os.path.abspath(__file__), '../secret_key.txt'))
try:
    f = open(key_path, 'r')
except FileNotFoundError:
    print("Secret Key file is missing!")
    exit(-1)

app = Flask(__name__)
api = Api(app)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///losslessapp.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['SECRET_KEY'] = f.read()
f.close()

db = SQLAlchemy(app)

class Users(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    public_id = db.Column(db.String(50), unique=True)
    username = db.Column(db.String(50))
    password = db.Column(db.String(80))
    balance = db.Column(db.Float)
    admin = db.Column(db.Boolean)
    record = db.relationship('Record', backref="users", lazy=True)

    def __repr__(self):
        return f"Public_id: {Users.public_id}, username: {Users.username}, balance: {Users.balance}, is admin: {Users.admin}"

class CryptoModel(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False)
    value = db.Column(db.Float, nullable=False)

    def __repr__(self):
        return f"ID: {CryptoModel.id}, name: {CryptoModel.name}, value: {CryptoModel.value}"

class CurrencyModel(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False)
    value = db.Column(db.Float, nullable=False)

    def __repr__(self):
        return f"ID: {CurrencyModel.id}, name: {CurrencyModel.name}, value: {CurrencyModel.value}"

class StockModel(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False)
    value = db.Column(db.Float, nullable=False)

    def __repr__(self):
        return f"ID: {StockModel.id}, name: {StockModel.name}, value: {StockModel.value}"

class RecordsType(enum.Enum):
    CryptoRecord = 1
    CurrencyRecord = 2
    StockRecord = 3

class Record(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    public_id = db.Column(db.String(50), db.ForeignKey('users.public_id'), nullable=False)
    record_type = db.Column(db.Enum(RecordsType), nullable=False)
    record_id = db.Column(db.Integer, nullable=False)
    quantity = db.Column(db.Float, nullable=False)
    price = db.Column(db.Float, nullable=False)
    date = db.Column(db.String(50), nullable=False)

    def __repr__(self):
        return f"Public_id: {Record.public_id}, record_type: {Record.record_type}, record_id: {Record.id}, \
            quantity: {Record.quantity}, price: {Record.price}, date: {Record.date}"

db.create_all()
def token_required(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        '''
        token = None

        if 'x-access-token' in request.headers:
            token = request.headers['x-access-token']

        if not token:
            abort(401, message="Missing authorization token! Please Sing In or Sign Up!")
        
        try:
            data = jwt.decode(token, app.config['SECRET_KEY'], algorithms=["HS256"])
            current_user = Users.query.filter_by(public_id=data['public_id']).first()
        except:
            abort(401, message="Token is invalid!")
        '''
        current_user = Users(
            public_id=str(uuid.uuid4()), 
            username="Admin", 
            password="Admin",
            balance=100000000.0,
            admin=True)
        
        return f(current_user, *args, **kwargs)
    return decorated

# Request parser for user sign up
signup_post_args = reqparse.RequestParser()
signup_post_args.add_argument("username", type=str)
signup_post_args.add_argument("password", type=str)
signup_post_args.add_argument("balance", type=float)

# Request parser for user sign in
signin_post_args = reqparse.RequestParser()
signin_post_args.add_argument("username", type=str)
signin_post_args.add_argument("password", type=str)

# [___]Model parser for adding new records into database
models_post_args = reqparse.RequestParser()
models_post_args.add_argument("name", type=str, required=True)
models_post_args.add_argument("value", type=float, required=True)

# Request parser for patch where we can modify any or both of the values
model_patch_args = reqparse.RequestParser()
model_patch_args.add_argument("name", type=str)
model_patch_args.add_argument("value", type=float)

record_buy_sell_args = reqparse.RequestParser()
record_buy_sell_args.add_argument("record_id", type=int)
record_buy_sell_args.add_argument("quantity", type=float)
record_buy_sell_args.add_argument("price", type=float)

serialize_fields = {
    'id': fields.Integer,
    'name': fields.String,
    'value': fields.Float
}

class SignUp(Resource):
    def post(self):
        args = signup_post_args.parse_args()
        if not args or not args['username'] or not args['password'] or not args['balance']:
            abort(400, message="Missing one or more fields!")
        result = Users.query.filter_by(username=args['username']).first()
        if result:
            abort(409, message="Username is already taken!")
        hashed_password = generate_password_hash(args['password'], method='sha256')

        new_user = Users(
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
        
        user = Users.query.filter_by(username=args['username']).first()

        if not user:
            abort(401, message="Wrong username or password or user does not exist!")
        
        if check_password_hash(user.password, args['password']):
            token = jwt.encode({'public_id': user.public_id, 'exp': datetime.datetime.utcnow() + \
                datetime.timedelta(minutes=120)},app.config['SECRET_KEY'],algorithm="HS256")
            return {"token": token}, 200
        
        # We get there only when password is wrong, but for safety issues we cannot say that to an user, so we display same result as when wrong username.
        abort(401, message="Wrong username or password or user does not exist!")

class Cryptocurrency(Resource):
    # GET result by id
    @token_required
    @marshal_with(serialize_fields)
    def get(current_user, self, crypto_id):
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if not result:
            abort(404, message="Crypto with that ID doesn't exist!")
        return result

    @token_required
    @marshal_with(serialize_fields)
    def post(current_user, self, crypto_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = models_post_args.parse_args()
        result = CryptoModel.query.filter_by(id=crypto_id).first()
        if result:
            abort(409, message="Crypto ID is already taken!")
        crypto = CryptoModel(id=crypto_id, name=args['name'], value=args['value'])
        db.session.add(crypto)
        db.session.commit()
        return crypto, 201

    @token_required
    @marshal_with(serialize_fields)
    def patch(current_user, self, crypto_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = model_patch_args.parse_args()
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
    @token_required
    @marshal_with(serialize_fields)
    def get(current_user, self):
        result = CryptoModel.query.all()
        if not result:
            abort(404, message="There are no cryptocurrencies in the database!")
        return result

class BuyCrypto(Resource):
    @token_required
    def post(current_user, self):
        args = record_buy_sell_args.parse_args()
        if not args["record_id"]:
            abort(400, message="Missing crypto id!")
        if not args["quantity"]:
            abort(400, message="Missing quantity!")
        if not args["price"]:
            abort(400, message="Missing buy price!")

        if args["quantity"] <= 0:
            abort(406, message="Quanity has to be bigger than 0!")

        if current_user.balance < args["quantity"] * args["price"]:
            abort(406, message="Insufficient account balance!")
        
        now = datetime.datetime.now()
        buyRecord = Record(
            public_id = current_user.public_id,
            record_type = RecordsType.CryptoRecord,
            record_id = args["record_id"],
            quantity  = args["quantity"],
            price = args["price"],
            date = now.strftime("%d.%m.%Y %H:%M:%S")
        )
        current_user.balance = current_user.balance - args["quantity"] * args["price"]
        db.session.add(buyRecord)
        db.session.commit()
        return {"message": "Succesfully bought crypto!"}, 201

class SellCrypto(Resource):
    @token_required
    def post(current_user, self):
        args = record_buy_sell_args.parse_args()
        if not args["record_id"]:
            abort(400, message="Missing crypto id!")
        if not args["quantity"]:
            abort(400, message="Missing quantity!")
        if not args["price"]:
            abort(400, message="Missing sell price!")

        if args["quantity"] <= 0:
            abort(406, message="Quantity has to be bigger than 0!")

        quantity_sum = 0
        user_records = Record.query.filter_by(
            public_id=current_user.public_id,
            record_type = RecordsType.CryptoRecord,
            record_id=args["record_id"]).all()
        for record in user_records:
            quantity_sum = quantity_sum + record.quantity
        
        if quantity_sum < args["quantity"]:
            abort(406, message="Insufficient cryptocurrency quantity!")
        
        now = datetime.datetime.now()
        sellRecord = Record(
            public_id = current_user.public_id,
            record_type = RecordsType.CryptoRecord,
            record_id = args["record_id"],
            quantity  = -1 * args["quantity"],
            price = args["price"],
            date = now.strftime("%d.%m.%Y %H:%M:%S")
        )
        current_user.balance = current_user.balance + args["quantity"] * args["price"]
        db.session.add(sellRecord)
        db.session.commit()
        return {"message": "Succesfully sold crypto!"}, 201

class CryptoRecords(Resource):
    @token_required
    def get(current_user, self):
        user_records = Record.query.filter_by(public_id=current_user.public_id, record_type=RecordsType.CryptoRecord).all()
        output = []
        for record in user_records:
            single_record = {}
            single_record["id"] = record.id
            single_record["public_id"] = record.public_id
            single_record["record_type"] = record.record_type.name
            single_record["record_id"] = record.record_id
            single_record["quantity"] = record.quantity
            single_record["price"] = record.price
            single_record["date"] = record.date
            output.append(single_record)
        return jsonify(output)

class Currency(Resource):
    @token_required
    @marshal_with(serialize_fields)
    def get(current_user, self, currency_id):
        result = CurrencyModel.query.filter_by(id=currency_id).first()
        if not result:
            abort(404, message="Currency with that ID doesn't exist!")
        return result

    @token_required
    @marshal_with(serialize_fields)
    def post(current_user, self, currency_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = models_post_args.parse_args()
        result = CurrencyModel.query.filter_by(id=currency_id).first()
        if result:
            abort(409, message="Currency ID is already taken!")
        currency = CurrencyModel(id=currency_id, name=args['name'], value=args['value'])
        db.session.add(currency)
        db.session.commit()
        return currency, 201

    @token_required
    @marshal_with(serialize_fields)
    def patch(current_user, self, currency_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = model_patch_args.parse_args()
        result = CurrencyModel.query.filter_by(id=currency_id).first()
        if not result:
            abort(404, message="Currency with that ID doesn't exist!")
        
        if args['name']:
            result.name = args['name']
        if args['value']:
            result.value = args['value']
        
        db.session.commit()
        return result, 200

    @token_required
    def delete(current_user, self, currency_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        result = CurrencyModel.query.filter_by(id=currency_id).first()
        if not result:
            abort(404, message="Currency with that ID doesn't exist!")
        db.session.delete(result)
        db.session.commit()
        return {"message": "Succesfully removed record from database."}, 200

class Currencies(Resource):
    @token_required
    @marshal_with(serialize_fields)
    def get(current_user, self):
        result = CurrencyModel.query.all()
        if not result:
            abort(404, message="There are no currencies in the database!")
        return result

class BuyCurrency(Resource):
    @token_required
    def post(current_user, self):
        args = record_buy_sell_args.parse_args()
        if not args["record_id"]:
            abort(400, message="Missing currency id!")
        if not args["quantity"]:
            abort(400, message="Missing quantity!")
        if not args["price"]:
            abort(400, message="Missing buy price!")

        if args["quantity"] <= 0:
            abort(406, message="Quantity has to be bigger than 0!")

        if current_user.balance < args["quantity"] * args["price"]:
            abort(406, message="Insufficient account balance!")
        
        now = datetime.datetime.now()
        buyRecord = Record(
            public_id = current_user.public_id,
            record_type = RecordsType.CurrencyRecord,
            record_id = args["record_id"],
            quantity  = args["quantity"],
            price = args["price"],
            date = now.strftime("%d.%m.%Y %H:%M:%S")
        )
        current_user.balance = current_user.balance - args["quantity"] * args["price"]
        db.session.add(buyRecord)
        db.session.commit()
        return {"message": "Succesfully bought currency!"}, 201

class SellCurrency(Resource):
    @token_required
    def post(current_user, self):
        args = record_buy_sell_args.parse_args()
        if not args["record_id"]:
            abort(400, message="Missing currency id!")
        if not args["quantity"]:
            abort(400, message="Missing quantity!")
        if not args["price"]:
            abort(400, message="Missing buy price!")

        if args["quantity"] <= 0:
            abort(406, message="Quantity has to be bigget than 0!")

        quantity_sum = 0
        user_records = Record.query.filter_by(
            public_id=current_user.public_id,
            record_type=RecordsType.CurrencyRecord, 
            record_id=args["record_id"]).all()
        for record in user_records:
            quantity_sum = quantity_sum + record.quantity
        
        if quantity_sum < args["quantity"]:
            abort(406, message="Insufficient currency quantity!")
        
        now = datetime.datetime.now()
        sellRecord = Record(
            public_id = current_user.public_id,
            record_type = RecordsType.CurrencyRecord,
            record_id = args["record_id"],
            quantity  = -1 * args["quantity"],
            price = args["price"],
            date = now.strftime("%d.%m.%Y %H:%M:%S")
        )
        current_user.balance = current_user.balance + args["quantity"] * args["price"]
        db.session.add(sellRecord)
        db.session.commit()
        return {"message": "Succesfully sold currency!"}, 201

class CurrenciesRecords(Resource):
    @token_required
    def get(current_user, self):
        user_records = Record.query.filter_by(public_id=current_user.public_id, record_type=RecordsType.CurrencyRecord).all()
        output = []
        for record in user_records:
            single_record = {}
            single_record["id"] = record.id
            single_record["public_id"] = record.public_id
            single_record["record_type"] = record.record_type.name
            single_record["record_id"] = record.record_id
            single_record["quantity"] = record.quantity
            single_record["price"] = record.price
            single_record["date"] = record.date
            output.append(single_record)
        return jsonify(output)


class Stock(Resource):
    @token_required
    @marshal_with(serialize_fields)
    def get(current_user, self, stock_id):
        result = StockModel.query.filter_by(id=stock_id).first()
        if not result:
            abort(404, message="Stock with that ID doesn't exist!")
        return result

    @token_required
    @marshal_with(serialize_fields)
    def post(current_user, self, stock_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = models_post_args.parse_args()
        result = StockModel.query.filter_by(id=stock_id).first()
        if result:
            abort(409, message="Stock ID is already taken!")
        stock = StockModel(id=stock_id, name=args['name'], value=args['value'])
        db.session.add(stock)
        db.session.commit()
        return stock, 201

    @token_required
    @marshal_with(serialize_fields)
    def patch(current_user, self, stock_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        args = model_patch_args.parse_args()
        result = StockModel.query.filter_by(id=stock_id).first()
        if not result:
            abort(404, message="Stock with that ID doesn't exist!")
        
        if args['name']:
            result.name = args['name']
        if args['value']:
            result.value = args['value']
        
        db.session.commit()
        return result, 200

    @token_required
    def delete(current_user, self, stock_id):
        if not current_user.admin:
            abort(401, message="Access denied!")
        result = StockModel.query.filter_by(id=stock_id).first()
        if not result:
            abort(404, message="Stock with that ID doesn't exist!")
        db.session.delete(result)
        db.session.commit()
        return {"message": "Succesfully removed record from database."}, 200

class Stocks(Resource):
    @token_required
    @marshal_with(serialize_fields)
    def get(current_user, self):
        result = StockModel.query.all()
        if not result:
            abort(404, message="There are no stocks in the database!")
        return result

class BuyStock(Resource):
    @token_required
    def post(current_user, self):
        args = record_buy_sell_args.parse_args()
        if not args["record_id"]:
            abort(400, message="Missing stock id!")
        if not args["quantity"]:
            abort(400, message="Missing quantity!")
        if not args["price"]:
            abort(400, message="Missing buy price!")

        if args["quantity"] <= 0:
            abort(406, message="Quanity has to be bigger than 0!")

        if current_user.balance < args["quantity"] * args["price"]:
            abort(406, message="Insufficient account balance!")
        
        now = datetime.datetime.now()
        buyRecord = Record(
            public_id = current_user.public_id,
            record_type = RecordsType.StockRecord,
            record_id = args["record_id"],
            quantity  = args["quantity"],
            price = args["price"],
            date = now.strftime("%d.%m.%Y %H:%M:%S")
        )
        current_user.balance = current_user.balance - args["quantity"] * args["price"]
        db.session.add(buyRecord)
        db.session.commit()
        return {"message": "Succesfully bought stock!"}, 201

class SellStock(Resource):
    @token_required
    def post(current_user, self):
        args = record_buy_sell_args.parse_args()
        if not args["record_id"]:
            abort(400, message="Missing stock id!")
        if not args["quantity"]:
            abort(400, message="Missing quantity!")
        if not args["price"]:
            abort(400, message="Missing buy price!")

        if args["quantity"] <= 0:
            abort(406, message="Quantity has to be bigger than 0!")

        quantity_sum = 0
        user_records = Record.query.filter_by(
            public_id=current_user.public_id,
            record_type=RecordsType.StockRecord, 
            record_id=args["record_id"]).all()
        for record in user_records:
            quantity_sum = quantity_sum + record.quantity
        
        if quantity_sum < args["quantity"]:
            abort(406, message="Insufficient stock quantity!")
        
        now = datetime.datetime.now()
        sellRecord = Record(
            public_id = current_user.public_id,
            record_type = RecordsType.StockRecord,
            record_id = args["record_id"],
            quantity  = -1 * args["quantity"],
            price = args["price"],
            date = now.strftime("%d.%m.%Y %H:%M:%S")
        )
        current_user.balance = current_user.balance + args["quantity"] * args["price"]
        db.session.add(sellRecord)
        db.session.commit()
        return {"message": "Succesfully sold stock!"}, 201

class StockRecords(Resource):
    @token_required
    def get(current_user, self):
        user_records = Record.query.filter_by(public_id=current_user.public_id, record_type=RecordsType.StockRecord).all()
        output = []
        for record in user_records:
            single_record = {}
            single_record["id"] = record.id
            single_record["public_id"] = record.public_id
            single_record["record_type"] = record.record_type.name
            single_record["record_id"] = record.record_id
            single_record["quantity"] = record.quantity
            single_record["price"] = record.price
            single_record["date"] = record.date
            output.append(single_record)
        return jsonify(output)

class Balance(Resource):
    @token_required
    def get(current_user, self):
        user_balance = { "balance" : current_user.balance }
        return jsonify(user_balance)

# Endpoints to access single cryptocurrency/currency/stock by their ID
api.add_resource(Cryptocurrency, "/cryptocurrencies/<int:crypto_id>")
api.add_resource(Currency, "/currencies/<int:currency_id>")
api.add_resource(Stock, "/stocks/<int:stock_id>")

# Endpoints for access to list of all cryptocurrencies/currencies/stocks
api.add_resource(Cryptocurrencies, "/cryptocurrencies")
api.add_resource(Currencies, "/currencies")
api.add_resource(Stocks, "/stocks")

# Endpoint for sign up 
api.add_resource(SignUp, "/register")

# Endpoint for sing in
api.add_resource(SignIn, "/login")

# Endpoints for buying cryptocurrencies/currencies/stocks
api.add_resource(BuyCrypto, "/cryptocurrencies/buy")
api.add_resource(BuyCurrency, "/currencies/buy")
api.add_resource(BuyStock, "/stocks/buy")

# Endpoints for selling cryptocurrencies/currencies/stocks
api.add_resource(SellCrypto, "/cryptocurrencies/sell")
api.add_resource(SellCurrency, "/currencies/sell")
api.add_resource(SellStock, "/stocks/sell")

# Endpoints for getting all records of cryptocurrencies/currencies/stocks for specific user
api.add_resource(CryptoRecords, "/cryptocurrencies/records")
api.add_resource(CurrenciesRecords, "/currencies/records")
api.add_resource(StockRecords, "/stocks/records")

# Endpoint for getting current user account balance
api.add_resource(Balance, "/balance")

if __name__ == "__main__":
    app.run(debug=True)