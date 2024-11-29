
public class inBridge {
    private final int N;
    private int currentThreads = 0;
    private int inTheBridge = 0;

    public inBridge(int N){
        this.N = N;
    }

    public synchronized void enterBridge(Car car) throws InterruptedException {
        this.inTheBridge++;
        if(this.inTheBridge > this.N){
            wait();
        }
        ColoredOutput.print("\nCar ("+ car.index + ") entered the bridge",car.color);
    }

    public synchronized void extiBridge(Car car) throws InterruptedException {
        this.inTheBridge--;
        car.monitor.inBridge--;
        if(this.inTheBridge >= this.N){
            notify();
        }
        ColoredOutput.print("\nCar ("+ car.index + ") exited the bridge",car.color);
    }
}
