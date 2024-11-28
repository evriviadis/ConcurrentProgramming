import java.util.Scanner;

public class App {

    public static void main(String[] args) throws Exception {
        if(args.length != 1){
            System.out.println("wrong number of arguments");
            System.exit(-1);
        }
        int index = 0;
        int maxSeats = Integer.parseInt(args[0]);
        Train train = new Train(maxSeats);

        train.start();

        Scanner scanner = new Scanner(System.in);
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

            train.newpassenger = number;
            for(int i=0; i<number ; i++){
                Passenger passenger = new Passenger(train, index);
                index++;
                passenger.start();
            }
        }

        System.out.println("Bye Bye Train");

        train.cntrl_D = true;
        train.notifi();
        scanner.close(); 
    }

}
