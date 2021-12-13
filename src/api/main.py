#!/usr/local/bin/python
"""
Author: Jake Mathai
Purpose: API endpoints
"""

import traceback
from datetime import datetime
from fastapi import FastAPI, WebSocket

from .bigint import Bigint, random_num

app = FastAPI()

@app.get('/generate/{size}')
def generate(size):
    response = {'result': ''}
    try:
        size = int(size)
        if size <= 0 or size > 1e8:
            raise Exception('Invalid size specified - must be positive but not ridiculously large')
        start = datetime.now()
        response['result'] = random_num(size)
        end = datetime.now()
        response['runtime_ms'] = (end - start).microseconds / 1000
    except Exception as e:
        response['result'] = f'failed: {e}'
    return response


@app.get('/div/{dividend}/{divisor}')
def div(dividend, divisor):
    response = {'result': ''}
    try:
        dividend = Bigint(dividend)
        divisor = Bigint(divisor)
        start = datetime.now()
        response['result'] = divisor.divides(dividend)
        end = datetime.now()
        response['runtime_ms'] = (end - start).microseconds / 1000
    except Exception as e:
        response['result'] = f'failed: {e}'
    return response


@app.get('/rand-div/{dividend_size}/{divisor_size}')
def rand_div(dividend_size, divisor_size):
    response = {'result': ''}
    try:
        dividend_size = int(dividend_size)
        divisor_size = int(divisor_size)
        if dividend_size < divisor_size:
            raise Exception('Dividend size must be >= divisor size')
        if dividend_size > 1e8 or divisor_size > 1e8:
            raise Exception('Ridiculous size requested, no thanks')
        dividend = Bigint(dividend_size, True)
        divisor = Bigint(divisor_size, True, True)
        response['dividend'] = dividend.as_str()
        response['divisor'] = divisor.as_str()
        response['dividend_size'] = dividend.size()
        response['divisor_size'] = divisor.size()
        start = datetime.now()
        response['result'] = divisor.divides(dividend)
        end = datetime.now()
        response['runtime_ms'] = (end - start).microseconds / 1000
        print('runtime:', response['runtime_ms'])
    except:
        response['result'] = f'Error: {traceback.format_exc()}'
    return response


@app.websocket('/ws/generate')
async def ws_generate(websocket: WebSocket):
    response = {'result': ''}
    await websocket.accept()
    while True:
        try:
            data = await websocket.receive_json()
            size = int(data['size'])
            if size <= 0 or size > 1e8:
                raise Exception('Invalid size specified - must be positive but not ridiculously large')
            start = datetime.now()
            response['result'] = random_num(size)
            end = datetime.now()
            response['runtime_ms'] = (end - start).microseconds / 1000
            await websocket.send_json(response)
        except Exception as e:
            response['result'] = f'failed: {e}'
        await websocket.send_json(response)


@app.websocket('/ws/div')
async def ws_div(websocket: WebSocket):
    response = {'result': ''}
    await websocket.accept()
    while True:
        try:
            data = await websocket.receive_json()
            dividend = Bigint(data['dividend'])
            divisor = Bigint(data['divisor'])
            start = datetime.now()
            response['result'] = divisor.divides(dividend)
            end = datetime.now()
            response['runtime_ms'] = (end - start).microseconds / 1000
            await websocket.send_json(response)
        except Exception as e:
            response['result'] = f'failed: {e}'
        await websocket.send_json(response)


@app.websocket('/ws/rand-div')
async def ws_rand_div(websocket: WebSocket):
    response = {'result': ''}
    await websocket.accept()
    while True:
        try:
            data = await websocket.receive_json()
            dividend_size = int(data['dividend_size'])
            divisor_size = int(data['divisor_size'])
            if dividend_size < divisor_size:
                raise Exception('Dividend size must be >= divisor size')
            if dividend_size > 1e8 or divisor_size > 1e8:
                raise Exception('Ridiculous size requested, no thanks')
            dividend = Bigint(dividend_size, True)
            divisor = Bigint(divisor_size, True, True)
            response['dividend'] = dividend.as_str()
            response['divisor'] = divisor.as_str()
            response['dividend_size'] = dividend.size()
            response['divisor_size'] = divisor.size()
            start = datetime.now()
            response['result'] = divisor.divides(dividend)
            end = datetime.now()
            response['runtime_ms'] = (end - start).microseconds / 1000
            await websocket.send_json(response)
        except Exception as e:
            response['result'] = f'failed: {e}'
        await websocket.send_json(response)
