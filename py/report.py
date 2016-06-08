#!/usr/bin/env python
#coding=utf8

import httplib, urllib

httpClient = None
try:
    report_data = {
        'auth_id': 1,
        'auth_key': '1eecf1c5e848ff42d88e5599b3e8dfc0',
        'device_id': 1,
        'report_id': 1,
        'payload': '{"field0": "4","field1": "0.4"}'
    }

    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    # httpClient = httplib.HTTPConnection("fat.fatmou.se", 80, timeout=30)
    httpClient = httplib.HTTPConnection("106.2.111.144", 80, timeout=30)

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
