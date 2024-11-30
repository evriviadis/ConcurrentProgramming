import java.lang.classfile.instruction.ThrowInstruction;
import java.util.LinkedList;
import java.util.Queue;

public class Train extends Thread{
    public int currentSeats = 0;
    public int fullSeats, newpassenger, inTrain = 0;
    public boolean startedTrain = false;
    public boolean cntrl_D = false;
    public boolean terminated = false;
    public final Queue<Passenger> waitingQueue = new LinkedList<>();
    public final Queue<Passenger> QcuzOverPass = new LinkedList<>();

    public Train(int fullSeats){
        this.fullSeats = fullSeats;
        this.startedTrain = startedTrain;
    }

    @Override
    public void run(){
        try {
            while(!cntrl_D){
                /* trainfun(); */

                this.startingTrip();

                if(this.startedTrain == false){
                    break;
                }

                System.out.println("\n---Starting Trip---\n");
                Thread.sleep(2000);
                System.out.println("\n---Ended Trip---\n");

                currentSeats -= fullSeats;

                this.endedTrip();

            }
            System.out.println("train terminated");
            terminate();
        } catch (Exception e) {
        }
    }

    public synchronized void startingTrip() throws InterruptedException{
        for(int i=0 ; i<fullSeats ; i++){
            notify();
        }
        
        if(this.inTrain != this.fullSeats){
            wait();
            if(cntrl_D){
                this.startedTrain = false;
                return;
            }
        }
        this.startedTrain = true;
    }


    public synchronized void endedTrip() throws InterruptedException{

        this.startedTrain = false;
        for(int i =0 ; i<fullSeats ; i++){
        
            Passenger passenger = addPasstoQ(null, 1);
            
            this.inTrain--;
            passenger.notifiPass();
        }
        Thread.sleep(100);
        System.out.println("\n\nPassengers left to be serviced: " + currentSeats + "\n\n");
    }

    public synchronized void terminate() throws InterruptedException {
        this.terminated = true;
        for(int i =0 ; i<this.inTrain ; i++){
        
            Passenger passenger = addPasstoQ(null, 1);
            passenger.notifiPass();
        }
    }


    public synchronized void getIn(Passenger pas) throws InterruptedException{
        this.currentSeats++;

        //System.out.println("currentSeats "+ this.currentSeats);

        while(this.inTrain >= this.fullSeats){
            
            //this.addPasstoQ2(pas, 0);
            //System.out.println("Passenger " + pas.index + "| Hi i didnt fit to the train train\n");
            wait();
        
            if(this.inTrain >= this.fullSeats){
                notify();
            }else{
                //System.out.println("\nPassenger: " + pas.index + " | Woked up!!");
            }

        }

        
        this.addPasstoQ(pas, 0);
        
        System.out.println("Passenger " + pas.index + "| Hi i have entered the train\n");

        this.inTrain++;
        if(this.inTrain == this.fullSeats){
            System.out.println("\nPassenger " + pas.index + " Woke the train\n");
            this.notifi();
        }
    }

    public synchronized Passenger addPasstoQ(Passenger pas, int operation) throws InterruptedException{
        if(operation == 0){
            waitingQueue.add(pas);
            return pas;
        }else if(operation == 1){
            Passenger newpass = waitingQueue.poll();
            return newpass;
        }else if(operation == 2){
            Passenger newpass = waitingQueue.peek();
            return newpass;
        }
        return pas;
    }

    public synchronized Passenger addPasstoQ2(Passenger pas, int operation) throws InterruptedException{
        if(operation == 0){
            QcuzOverPass.add(pas);
            return pas;
        }else if(operation == 1){
            Passenger newpass = QcuzOverPass.poll();
            return newpass;
        }else if(operation == 2){
            Passenger newpass = QcuzOverPass.peek();
            return newpass;
        }
        return pas;
    }

    public synchronized void notifi() throws InterruptedException{
        notify();
    }
}