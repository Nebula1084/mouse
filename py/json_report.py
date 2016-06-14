#!/usr/bin/env python
#coding=utf8

import urllib2
import json

try:
    report_data = {
        'auth_id': 1,
        'auth_key': 'e4d3dc84ef92b76c71e3faf983e02c47',
        'device_id': 1,
        'report_id': 1,
        'payload': {'field0': 0.5, 'field1': 10, 'field2': 'ABCDEFGH'}
    }

    report_json = json.dumps(report_data)
    url = "http://fat.fatmou.se/api/report"

    req = urllib2.Request(url, report_json)
    req.add_header('Accept', 'application/json')
    req.add_header('Content-Type', 'application/json')

    response = urllib2.urlopen(req)

    print response.read()
except Exception, e:
    print e
