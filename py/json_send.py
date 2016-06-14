#!/usr/bin/env python
#coding=utf8

import urllib2
import json

try:
    report_data = {
        'control_id': 1,
        'payload': [
            {'name': 'field0', 'value': 10},
        ]
    }

    report_json = json.dumps(report_data)
    url = "http://fat.fatmou.se/api/push?device_id=XXXX" #fill with your id

    req = urllib2.Request(url, report_json)
    req.add_header('Accept', 'application/json')
    req.add_header('Content-Type', 'application/json')
    response = urllib2.urlopen(req)

    print response.read()
except Exception, e:
    print e
