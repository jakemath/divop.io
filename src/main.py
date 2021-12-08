#!/usr/local/bin/python
"""
Author: Jake Mathai
Purpose: API entrypoint
"""

import traceback
from copy import deepcopy
from bigint import Bigint
from fastapi import FastAPI, WebSocket

app = FastAPI()

@app.get('/')
def index():
    return {'message': 'hi'}

@app.get('/generate/{size}')
def generate(size):
    response = {'response': ''}
    try:
        size = int(size)
        if size <= 0:
            raise Exception('Invalid size specified - must be positive')
        response['response'] = Bigint(size, True).as_str()
    except:
        response['response'] = traceback.format_exc()
    return response

@app.get('/div/{dividend}/{divisor}')
def div(dividend, divisor):
    response = {'result': ''}
    try:
        dividend = Bigint(dividend)
        divisor = Bigint(divisor)
        response['result'] = divisor.divides(dividend)
    except:
        response['result'] = f'Error: {traceback.format_exc()}'
    return response


@app.get('/divrandom/{dividend_size}/{divisor_size}')
def divrandom(dividend_size, divisor_size):
    response = {'result': ''}
    try:
        dividend_size = int(dividend_size)
        divisor_size = int(divisor_size)
        if dividend_size < divisor_size:
            raise Exception('Dividend size must be >= divisor size')
        dividend = Bigint(dividend_size, True)
        divisor = Bigint(divisor_size, True)
        response['dividend'] = deepcopy(dividend.as_str())
        response['divisor'] = deepcopy(divisor.as_str())
        response['result'] = divisor.divides(dividend)
    except:
        response['result'] = f'Error: {traceback.format_exc()}'
    return response


@app.websocket('/ws/div')
async def ws_div(websocket: WebSocket):
    await websocket.accept()
    while True:
        data = await websocket.receive_json()
        request_id = data['request_id']
        response = {'request_id': request_id}
        dividend = Bigint(data['dividend'])
        divisor = Bigint(data['divisor'])
        response['result'] = divisor.divides(dividend)
        await websocket.send_json(response)
