import math
import random
import sys
import os
from datetime import datetime


class Record(object):
    """Represents a record."""

class Donations(Record):
    """Represents a record."""

class Table(object):
    """Reads input data."""
    def __init__(self):
        self.records = []

    def __len__(self):
        return len(self.records)

    def ReadFile(self, data_dir,  constructor, n=None):
        fp = open(data_dir)
        for i, line in enumerate(fp):
            if i == n:
                break
            inner_list = [elt.strip() for elt in line.split('|')]
            s = inner_list[13]
            try:
                date = datetime(year=int(s[4:8]), month=int(s[0:2]), day=int(s[2:4]))
            except:
                continue

            zip = inner_list[10]
            zip_dgts = zip[0:5]

            if (len(zip_dgts)< 5 or len(zip_dgts) > 11):
                continue;

            if(inner_list[15] != ''):
                continue;
            else:
                inner_list[15] = 'empty'

            if (inner_list[0]=='' or inner_list[14]==''):
                continue;

            """ Reading Fileds from input file """
            fields = [
                ('CMTE_ID', inner_list[0]),
                ('NAME', inner_list[7]),
                ('ZIP_CODE', zip_dgts),
                ('TRANSACTION_DT', date),
                ('TRANSACTION_AMT', inner_list[14]),
                ('OTHER_ID', inner_list[15]),
                 ]
            record = self.MakeRecord(line, fields, constructor)
            self.AddRecord(record)
        fp.close()

    def MakeRecord(self, line, fields, constructor):
        obj = constructor()
        for (field, val) in fields:
            setattr(obj, field, val)
        return obj

    def AddRecord(self, record):
        self.records.append(record)


class Donors(Table):
    def ReadRecords(self, data_dir, n=None):
        self.ReadFile(data_dir, Donations, n)

def PrintInput(table):
    for p in table.records:
        print ("CMTE_ID: ", p.CMTE_ID)
        print ("NAME: ", p.NAME)
        print ("Zip Code: ", p.ZIP_CODE)
        print ("TRANSACTION_DT: ", p.TRANSACTION_DT)
        print ("TRANSACTION_AMT: ", p.TRANSACTION_AMT)
        print ("OTHER_ID: ", p.OTHER_ID)
        print ('**********************************************')


def percentile(N,P):
    N.sort()
    nn = int(round(P * len(N) + 0.5))
    return N[nn-1]


def ProcessReciiepients(cmt, zip, sing_amount, p_input, Rec):
    """Calculates Repeated Recipents ID, amount, percentine and Number of donations to one recipient."""

    PP=p_input/100.
    amt=0.
    NN=[]
    pcnt = sing_amount
    n=1
    for q in Rec.records:
        if (cmt== q.CMTE_ID and zip == q.ZIP_CODE):
            ppnt = float(q.TRANSACTION_AMT)
            amt += float(q.TRANSACTION_AMT)
            NN.append(q.TRANSACTION_AMT)
            n += 1
    if(len(NN) > 0):
        pcnt = percentile(NN,PP)
    return amt, pcnt, n

def MakeTables(data_dir):
    table = Donors()
    table.ReadRecords(data_dir)
    return table

def GetPercentile(percentile_dir):
    fp = open(percentile_dir)
    p=50.
    for i, line in enumerate(fp):
        p = int(line)
    return p

def main(name, data_dir='', percentile_dir='', out_dir=''):

    table = MakeTables(data_dir)
    pnt_input = GetPercentile(percentile_dir)
    #    PrintInput(table)   ### Uncomment For printing input fields

    """Calculates Repeated Donors."""

    Repeaters = Donors()
    i=0
    names =[]
    for p in table.records[:-1]:
        if any(p.NAME in item for item in names):
            continue;
        names.append(p.NAME)
        t=None                
        i += 1
        for q in table.records[i:]:               
            if (p.NAME == q.NAME and p.ZIP_CODE == q.ZIP_CODE ):
                t=p
                if (p.TRANSACTION_DT.year < q.TRANSACTION_DT.year):
                    t=q
                p=t
                    
        if(t!= None ):
            Repeaters.AddRecord(t)


    """Calculates Repeated Recipients."""

    Recipients = Donors()
    f = open(out_dir,"w")
    for p in Repeaters.records:
        amount, prcntl, num = ProcessReciiepients(p.CMTE_ID, p.ZIP_CODE, p.TRANSACTION_AMT, pnt_input, Recipients)
        totamount = int(float(p.TRANSACTION_AMT)+amount)
        f.write(p.CMTE_ID+"|"+p.ZIP_CODE+"|"+str(p.TRANSACTION_DT.year)+"|"+prcntl+"|"+str(totamount)+"|"+str(num)+"\n")
        Recipients.AddRecord(p)
    f.close()

if __name__ == '__main__':
    import sys
    main(*sys.argv)
