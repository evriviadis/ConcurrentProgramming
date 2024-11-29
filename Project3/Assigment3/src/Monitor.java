
public class Monitor {
    public boolean isChoosing = false;
    private String priority = "NONE", flow = "NONE";
    public int inBridge = 0, passed = 0;
    public int redCars, blueCars, N;

    public Monitor(int N, int redCars, int blueCars){
        this.redCars = redCars;
        this.blueCars = blueCars;
        this.N = N;
    }

    public synchronized void givePrio() throws InterruptedException{

        if(flow.equals("BLUE")){
            this.priority = "RED";
        }else{
            this.priority = "BLUE";
        }
    }

    public synchronized void desicion(Car car) throws InterruptedException {
        while (this.isChoosing) {
            wait();
        }

        this.isChoosing = true;

        if(this.priority.equals(car.color) && this.inBridge == 0){
           
            this.passed = 0;
            this.flow = car.color;
            this.priority = "NONE";
            this.inBridge++;
            this.passed++;
            
            this.isChoosing = false;
            notify();
            
            car.entered = true;

        }else if(!this.priority.equals("NONE") && this.inBridge > 0){

            this.isChoosing = false;
            notify();

        }else if(this.passed > (N)){

            this.givePrio();
            this.passed = 0;
            this.flow = "NONE";

            this.isChoosing = false;
            notify();

        }else if(this.flow.equals(car.color)){

            this.inBridge++;
            this.passed++;
           
            this.isChoosing = false;
            notify();
            
            car.entered = true;

        }else if(this.inBridge == 0 && this.priority.equals("NONE")){

                this.passed = 0;
                this.flow = car.color;
                this.inBridge++;
                this.passed++;
                
                this.isChoosing = false;
                notify();
                
                car.entered = true;
        
        }else{
            
            this.isChoosing = false;
            notify();

        }
    }
}