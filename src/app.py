from flask import Flask, request, jsonify
import keystorage
app = Flask(__name__)

@app.route('/kvs_lib', methods=['POST'])
def handle_system():
    #POST: Create new KVS of specified name
    if request.method == 'POST':
        body = request.json
        if body is None:
            return jsonify({'error': 'Invalid JSON'}), 400
        try:
            k = keystorage.KeyStorage()
            k.initialize('../kvs_lib')
            k.create_kvs(body['name'])
            return jsonify({'kvs name':body['name']}), 201
        except Exception as e:
            return jsonify({'error': f'Malformed request: {str(e)}'}), 400


@app.route('/kvs_lib/<name>', methods=['POST'])
def handle_kvs(name):
    #POST: Create new K:V pair into this KVS
    if request.method == 'POST':
        body = request.json
        if body is None:
            return jsonify({'error': 'Invalid JSON'}), 400
        try:
            k = keystorage.KeyStorage()
            k.initialize('../kvs_lib')
            k.add(name, body['key'], body['value'])
            return jsonify({'key':body['key'], 'value':body['value']}), 201
        except Exception as e:
            return jsonify({'error': f'Malformed request: {str(e)}'}), 400

@app.route('/kvs_lib/<name>/<key>', methods=['GET', 'PUT', 'DELETE'])
def handle_key(name, key):
    #GET: Get value listed from key and KVS it belongs to
    #PUT: Replace value in key with new value
    #DELETE: Deletes current key
    if request.method == 'GET':
        k = keystorage.KeyStorage()
        k.initialize('../kvs_lib')
        data = {
            "key": key,
            "value": k.find(name, key),
            "_links": {
                "self": {"href": f"/kvs_lib/{name}/{key}"},
                "kvs": {"href": f"/kvs_lib/{name}"}
            }
        }
        return jsonify(data)
    elif request.method == 'PUT':
        body = request.json
        if body is None:
            return jsonify({'error': 'Invalid JSON'}), 400
        try:
            k = keystorage.KeyStorage()
            k.initialize('../kvs_lib')
            k.update(name, body['key'], body['value'])
            return jsonify({'key':body['key'], 'updated value':body['value']}), 201
        except Exception as e:
            return jsonify({'error': f'Malformed request: {str(e)}'}), 400
    elif request.method == 'DELETE':
        try:
            k = keystorage.KeyStorage()
            k.initialize('../kvs_lib')
            k.remove(name, key)
            return jsonify({'deleted key':key}), 200
        except Exception as e:
            return jsonify({'error': f'Malformed request: {str(e)}'}), 400


if __name__ == '__main__':
    app.run()