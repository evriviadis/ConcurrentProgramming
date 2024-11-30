import java.io.File;
import java.util.Scanner;
//import java.io.File;

public class App {
    public static void main(String[] args) throws Exception {
        if(args.length != 1){
            System.out.println("Wrong number of arguments");
            System.exit(-1);
        }

        int numberOfWorkers = Integer.parseInt(args[0]);

        Monitor monitor = new Monitor(1);

      

        for(int i=0; i<numberOfWorkers ; i++){
            Worker newWorker = new Worker(i,monitor);
            newWorker.start();
        }


        //FOR INPUT FROM A FILE
        Scanner scanner = new Scanner(new File("C:/Users/evris/ConcurrentProgramming/Project3/input.txt"));
        
        
        
        //FOR INPUT FROM TERMINAL TEXT
        //Scanner scanner = new Scanner(System.in);
        int number = 0;

        System.out.println("Enter numbers (Ctrl+D to end on Linux/Mac, Ctrl+Z on Windows):");
        while (scanner.hasNext()) {
            // Check if the input is a number
            if (scanner.hasNextInt()) {
                number = scanner.nextInt();
                System.out.println("You entered: " + number);
            } else {
                // If not a number, consume the invalid input
                String invalidInput = scanner.next();
                System.out.println("Invalid input, not a number: " + invalidInput);
                continue;
            }

            try {
                monitor.assignNumber(number);
            } catch (Exception e) {
            }
        }
        monitor.terminate();
        //System.out.println("\n\n---Thanks for you time! Bye Bye---\n");
        //System.exit(1);
    }
}
