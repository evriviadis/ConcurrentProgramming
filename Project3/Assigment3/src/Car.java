
public class Car extends Thread {
    public String color;
    public int index;
    public boolean entered = false;
    public final Monitor monitor;
    private final inBridge bridge;
    
    public Car(Monitor monitor, String color, int index, inBridge bridge){
        this.monitor = monitor;
        this.color = color;
        this.index = index;
        this.bridge = bridge;
    }

    @Override
    public void run(){
        while(this.entered == false){ 
            try{
                monitor.desicion(this);
                if(this.entered){
            
                    bridge.wannaEnter(this);
                    monitor.exitbridge();

                }

            }catch (Exception e) {
            }   
        }
        //ColoredOutput.print("car " + this.index + " exited the bridge succesffully", this.color);
    }

    public synchronized void sleepy() {
        try {
            Thread.sleep(1000);
        } catch (Exception e) {
        }
    }
}
