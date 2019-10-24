from __future__ import print_function
import pickle
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
import pandas as pd
import pygsheets
import csv

# If modifying these scopes, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/spreadsheets.readonly']

# The ID and range of a sample spreadsheet.
#SAMPLE_SPREADSHEET_ID = '1VA32JL3lmicAF6JE3nLDzDSLWd07U6S72xDkzdTBVZA'
SAMPLE_SPREADSHEET_ID = '1qnCxA6FPIh1Y5w-cG3LFzdPnkVu2b0p14_viVjkDldg'
#SAMPLE_RANGE_NAME = 'Sheet1!A1:F7000'
SAMPLE_RANGE_NAME = 'ScintillationDataDump!A1:F7000'

def main():

    creds = None
   
    if os.path.exists('token.pickle'):
        with open('token.pickle', 'rb') as token:
            creds = pickle.load(token)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'credentials.json', SCOPES)
            creds = flow.run_local_server()
        # Save the credentials for the next run
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)

    service = build('sheets', 'v4', credentials=creds)

    # Call the Sheets API
    sheet = service.spreadsheets()
    gsheet = sheet.values().get(spreadsheetId=SAMPLE_SPREADSHEET_ID,
                                range=SAMPLE_RANGE_NAME).execute()
    values = gsheet.get('values', [])


    header = gsheet.get('values', [])[0]   # Assumes first line is header!
    values = gsheet.get('values', [])[1:]  # Everything else is data.

    
    nrows = 2
    collnumber = 0
    all_data = []
    for col_id, col_name in enumerate(header):
       column_data = []
       for row in values:
 #          column_data.append(row[col_id])
 #          print(row[col_id])
           if collnumber == 0:
               nrows = nrows+1
 #      ds = pd.Series(data=column_data, name=col_name)
#       all_data.append(ds)
       collnumber = collnumber + 1
   # df = pd.concat(all_data, axis=1)

    print(nrows)
    colums = []
    iter = 0
    df = []
    with open('blocks.csv') as csvfile:
        readCSV = csv.reader(csvfile, delimiter=',')
        for row in readCSV:
            if iter == 0:
                colums = row
                print(colums)
                df = pd.DataFrame(columns=row)
            elif iter > 0:
                df = df.append({colums[0]:row[0],colums[1]:row[1],colums[2]:row[2],colums[3]:row[3],colums[4]:row[4],colums[5]:row[5]},ignore_index=True)

            iter = iter + 1





    gc = pygsheets.authorize(service_file='TestProject.json')



    sh = gc.open('Blocks database')

    #select the first sheet 
    wks = sh.worksheet('title','ScintillationDataDump')


    print(wks)
    #update the first sheet with df, starting at cell A1 
    wks.set_dataframe(df,(nrows,1))

            
if __name__ == '__main__':
    main()
