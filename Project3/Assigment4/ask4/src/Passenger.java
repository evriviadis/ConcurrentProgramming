

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
        
        System.out.println("Passenger " + this.index+ " | I exited the train\n");
    }

    public synchronized void waitPass() throws InterruptedException{

        System.out.println("Im waiting here");
        wait();

    }

    public synchronized void notifiPass() throws InterruptedException{
        //System.out.println("Passenger: " + this.index + " | Waked up");
        notify();
    }
}

