

class Passenger extends Thread {
    public Train train;
    public int index;
    
    public Passenger(Train train, int index){
        /*Initialization of passenger */
        this.train = train;
        this.index = index;
    }

    public void run(){
        /*Get in the train*/
        try {
            train.getIn(this);
        } catch (Exception e) {
        }

    
        //
        /*Wait for train to end the trip */
        try{
            this.waitForTrain();
        }catch (Exception e){
        }

        

        /*Get off the train */
        
    }

    public synchronized void waitForTrain() throws InterruptedException{
        
        wait();

        if(this.train.cntrl_D){

            System.out.println("Passenger "+ this.index + " left unsatisfied :(");
            
        }else{

            System.out.println("Passenger " + this.index+ " | I exited the train\n");

        }
    }

    public synchronized void notifiPass() throws InterruptedException{
        notify();
    }
}

