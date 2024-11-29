
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

                    bridge.enterBridge(this);
                    Thread.sleep(2000);
                    bridge.extiBridge(this);

                }

            }catch (Exception e) {
            }   
        }
    }
}
