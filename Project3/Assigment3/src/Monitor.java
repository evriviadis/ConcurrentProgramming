import java.util.LinkedList;
import java.util.Queue;

public class Monitor {
    public boolean isChoosing = false;
    private String priority = "NONE", flow = "NONE";
    private int inBridge = 0, passed = 0;
    public int redCars, blueCars, N;
    private inBridge bridge;
    public final Queue<Car> waitingQueue = new LinkedList<>();

    public Monitor(int N, int redCars, int blueCars, inBridge bridge){
        this.redCars = redCars;
        this.blueCars = blueCars;
        this.N = N;
        this.bridge = bridge;
    }

    public synchronized void givePrio() throws InterruptedException{
        System.out.println("HI IM IN GIVE PRIO ");

        if(flow.equals("BLUE")){
            this.priority = "RED";
        }else{
            this.priority = "BLUE";
        }
    }

    public synchronized void exitbridge() throws InterruptedException{
        this.inBridge--;
    }

    public synchronized void desicion(Car car) throws InterruptedException {
        while (this.isChoosing) {
            wait();
        }

        this.isChoosing = true;

        
        //ColoredOutput.print("flow", this.flow);

        if(this.priority.equals(car.color) && this.inBridge == 0){

            ColoredOutput.print("Car " + car.index + " entered Gate 1", car.color);
            /* mppes sto bridge */
            //inBridge++;
            //flow.eqals(car.color);
            //if(flow.equals("RED")){
            //      redCars--;
            //  }else{
            //      blueCars--;
            //  }
            //passed ++;

            this.passed = 0;
            this.flow = car.color;
            this.priority = "NONE";
            this.inBridge++;
            this.passed++;
            
            this.isChoosing = false;
            notify();
            
            car.entered = true;
            waitingQueue.add(car);


        }else if(!this.priority.equals("NONE") && this.inBridge > 0){

            //ColoredOutput.print("Car " + car.index + " entered Gate 2", car.color);
            /* edo kanei apla notify all kai bgeneis esi */

            this.isChoosing = false;
            notify();
        }else if(this.passed > (2*N)){

            //ColoredOutput.print("Car " + car.index + " entered Gate 3", car.color);
            /* dose prio stin alli pleura */
            

            this.givePrio();
            this.passed = 0;
            this.inBridge = 0;
            this.flow = "NONE";


            this.isChoosing = false;
            notify();

        }else if(this.flow.equals(car.color)){

            ColoredOutput.print("Car " + car.index + " entered Gate 4", car.color);
            
            /* exeis to flow opote mpes  */

    
            this.inBridge++;
            this.passed++;
           
            this.isChoosing = false;
            notify();
            
            car.entered = true;
            waitingQueue.add(car);


        }else if(this.inBridge == 0 && this.priority.equals("NONE")){

                ColoredOutput.print("Car " + car.index + " entered Gate 5", car.color);

                /* mpes mesa */

                this.passed = 0;
                this.flow = car.color;
                this.inBridge++;
                this.passed++;
                
                this.isChoosing = false;
                notify();

                
                car.entered = true;
                waitingQueue.add(car);
        }else{
            this.isChoosing = false;
            notify();
        }
    }

    public synchronized Car nextCar() {
        return waitingQueue.poll();
    }

    public synchronized Car pickCar() {
        return waitingQueue.peek();
    }



    //AYTO TO FUNCTION EINAI APLA GIA TESTING 
    public synchronized void count(int number) throws InterruptedException{
        System.out.println("Im waitting....");
        wait();
        for(int i=0 ; i<number ; i++){
            System.out.println(i);
            Thread.sleep(1000);
        }
        notify();
    }


    //AYTO EDO EINAI GIA KANEIS NOTIFY APO TIN MAIN
    public synchronized void notifi() throws InterruptedException{
        notifyAll();
    }


    public synchronized void printQueue() {
        if (waitingQueue.isEmpty()) {
            System.out.println("Queue is empty.");
        } else {
            System.out.print("Current queue: ");
            for (Car car : waitingQueue) {
                System.out.print("Car " + car.index + " (" + car.color + ") -> ");
            }
            System.out.println("END");
        }
    }
}