public class Monitor{
    public int numberToAssign;
    public boolean givenJob = false;
    private int id;
    public boolean isTerminated = false;

    public Monitor(int id){
        this.id = id;
    }
    
    public synchronized void terminate() {
        isTerminated = true;
        notifyAll(); // Wake up any waiting threads
    }

    public synchronized boolean isTerminated() {
        return isTerminated;
    }

    public synchronized void assignNumber(int number) throws InterruptedException{
        while(givenJob){
            wait();
        }

        numberToAssign = number;

        givenJob = true;
        notify();
    }

    public synchronized int grabNumber() throws InterruptedException{
        while(!givenJob && !this.isTerminated){
            wait();
        }

        givenJob = false;
        notify();

        return numberToAssign;
    }
}
