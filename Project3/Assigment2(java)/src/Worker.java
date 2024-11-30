public class Worker extends Thread{
    public int id;
    public Monitor monitor;
    public int nubmerToCheck;

    public Worker(int id, Monitor monitor){
        this.id = id;
        this.monitor = monitor;
    }

    public void run(){
        try{
            while(true){
                nubmerToCheck = this.monitor.grabNumber();
                if(this.monitor.isTerminated()){
                    break;
                }
                isPrime(nubmerToCheck);
            }


        } catch (Exception e) {
        }
        System.out.println("Worker " + this.id + " is terminating.");
    }

    public static boolean isPrime(int number) {
        if (number <= 1) {
            System.out.println(number + " is not a prime number.");
            return false;
        }
        if (number == 2) {
            System.out.println(number + " is a prime number.");
            return true;
        }
        if (number % 2 == 0) {
            System.out.println(number + " is not a prime number.");
            return false;
        }

        
        for (int i = 3; i <= Math.sqrt(number); i += 2) {
            if (number % i == 0) {
                System.out.println(number + " is not a prime number.");
                return false;
            }
        }

        System.out.println(number + " is a prime number.");
        return true;
    } 

}
