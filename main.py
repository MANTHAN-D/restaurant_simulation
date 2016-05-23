#!/usr/bin/python

from Tkinter import *
import Tkinter
import threading
import RestaurantApp
import time

top = Tk()

def print_time():
   print "In Completed"
   orders = RestaurantApp.getCompletedTaskStatus()
   orders1 = orders.split("\n")
   for order in orders1:
      Lb1.insert(END,order) 
   print "Out Completed"
   # time.sleep(3)

def start_app():
    try:
        #t1=Process(target=print_time)
        #t1.start()
        t = threading.Thread(target=RestaurantApp.startApp)
        t.start()
        #RestaurantApp.getPendingTaskStatus()
        #print RestaurantApp.getCompletedTaskStatus()
    except:
        print "Error: unable to start thread"


RestaurantApp.setTaskBoard();

# Code to add widgets will go here...
def insertCallBack():
    Lb1.insert(END, E1.get())


def deleteCallBack():
    Lb1.delete(END)

B1 = Tkinter.Button(top,text="Start", command=start_app)
B1.pack()

B2 = Tkinter.Button(top,text="Update", command=print_time)
B2.pack()

Lb1 = Listbox(top,width=50,height=40)

Lb1.pack()

#Lb2 = Listbox(bottomframe)

#Lb2.pack( side = RIGHT )

top.mainloop()

