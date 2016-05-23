import threading
import RestaurantApp
import time
from multiprocessing import Process

def print_time():
  count=0
  while count<12:
    print "In Pending"
    print RestaurantApp.getCompletedTaskStatus()
    print "Out Pending"
    time.sleep(3)
    count += 1
    
# Create two threads as follows
try:
   RestaurantApp.setTaskBoard();
   print "Holllllaaaaaa"
   #RestaurantApp.startApp();
   t1 = threading.Thread(target=print_time)
   t1.start()
   t = threading.Thread(target=RestaurantApp.startApp)
   t.start()
   print "oki"
   #RestaurantApp.getPendingTaskStatus()
   print "poki"
   #time.sleep(7)
   print "--------------------------Holllllaaaaaa----------------------"
  
   #print RestaurantApp.getPendingTaskStatus()
   print "--------------------------PPPPolllllaaaaaa----------------------"
   print RestaurantApp.getCompletedTaskStatus()
except:
   print "Error: unable to start thread"

