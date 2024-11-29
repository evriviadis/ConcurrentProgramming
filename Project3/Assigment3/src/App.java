public class App {
    public static void main(String[] args) throws Exception {
        if(args.length != 3){
            System.out.println("wrong number of arguments");
            System.exit(-1);
        }
        int N = Integer.parseInt(args[0]);
        int redCars = Integer.parseInt(args[1]);
        int blueCars = Integer.parseInt(args[2]);

        inBridge bridge = new inBridge(N);
        Monitor monitor = new Monitor(N, redCars, blueCars);
    
        for(int i=0 ; i<redCars ; i++){
            Car threadCar = new Car(monitor, "RED", i, bridge);
            threadCar.start();
        }
        for(int i=0 ; i<blueCars ; i++){
            Car threadCar = new Car(monitor, "BLUE", i, bridge);
            threadCar.start();    
        }
    }
}

