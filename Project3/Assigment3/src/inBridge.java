
public class inBridge {
    private final int N;
    private int currentThreads = 0;

    public inBridge(int N){
        this.N = N;
    }

    public void wannaEnter(Car car) throws InterruptedException{
        Thread.sleep(1000);
        
        
        
        synchronized (this) {
            Car carInQ =  car.monitor.pickCar();

            ColoredOutput.print("Thread_Car " + car.index, car.color);
            ColoredOutput.print("1. Thread_Car first in the q = " + carInQ.index, carInQ.color);

            while((car.index != carInQ.index) || !(car.color.equals(carInQ.color))){
                System.out.print("reeeeee edoo kolisame" + car.index + "\n");

                if (car.monitor.waitingQueue.isEmpty()) {
                    notifyAll(); // Ξύπνα όλα τα threads
                    return; // Τερμάτισε τη συνάρτηση αν δεν υπάρχουν άλλα cars
                }
                
                wait();
            }

            notify();


            ColoredOutput.print("2. Thread_Car first in the q = " + carInQ.index, carInQ.color);
            ColoredOutput.print("I passed (Thread_Car " + car.index + ")", car.color);
            
            car.monitor.waitingQueue.remove();

            while (currentThreads >= N) {
                wait();
            }
            currentThreads++;
        }

        //ColoredOutput.print("Car " + car.index + " entering the bridge", car.color);
        
        car.sleepy();
        
        //car.monitor.printQueue();
        ColoredOutput.print("Car " + car.index + " exited the bridge", car.color);
        
        synchronized (this) {
            currentThreads--;
            
            notifyAll();
        }
    }

    private synchronized boolean isCarNext(Car car) {
        Monitor monitor = car.monitor;
        return car == monitor.nextCar(); 
    }
}
