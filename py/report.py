#!/usr/bin/env python
#coding=utf8

import httplib, urllib
import json

httpClient = None
try:
    payload = {'field0': 0.5, 'field1': 20, 'field2': 'ABCDEFGH'}
    report_data = {
        'auth_id': 3,
        'auth_key': 'a04e820a8c636ba1eb121f6463d418ff',
        'device_id': 3,
        'report_id': 4,
        'payload': json.dumps(payload),
    }

    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    httpClient = httplib.HTTPConnection("fat.fatmou.se", 80, timeout=30)

    params = urllib.urlencode(report_data)
    httpClient.request("POST", "/api/report", params, headers)

    response = httpClient.getresponse()
    print response.status
    print response.reason
    print response.read()
    print response.getheaders()
except Exception, e:
    print e
finally:
    if httpClient:
        httpClient.close()
