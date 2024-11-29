public class Monitor{
    public int numberToAssign;
    public boolean givenJob = false;
    private int id;

    public Monitor(int id){
        this.id = id;
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
        while(!givenJob){
            wait();
        }

        givenJob = false;
        notify();

        return numberToAssign;
    }
}
