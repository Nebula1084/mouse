#!/usr/bin/env python
#coding=utf8

import httplib, urllib

httpClient = None
try:
    recv_control_data = {
        'auth_id': 3,
        'auth_key': 'a04e820a8c636ba1eb121f6463d418ff',
        'device_id': 3,
        'control_id': 2,
        'sr' : 'R',
    }

    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    httpClient = httplib.HTTPConnection("fat.fatmou.se", 80, timeout=30)

    params = urllib.urlencode(recv_control_data)
    httpClient.request("POST", "/api/control", params, headers)

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