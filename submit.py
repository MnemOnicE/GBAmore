import urllib.request
import json
import sys

def submit():
    try:
        req = urllib.request.Request(
            'http://127.0.0.1:8080/submit',
            method='POST',
            headers={'Content-Type': 'application/json'},
            data=json.dumps({}).encode('utf-8')
        )
        with urllib.request.urlopen(req) as response:
            print("Successfully submitted:", response.read().decode('utf-8'))
    except Exception as e:
        print("Failed to submit:", e, file=sys.stderr)
        sys.exit(1)

if __name__ == '__main__':
    submit()
