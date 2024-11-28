public class ColoredOutput {
    public static final String RESET = "\u001B[0m";
    public static final String RED = "\u001B[31m";
    public static final String GREEN = "\u001B[32m";
    public static final String YELLOW = "\u001B[33m";
    public static final String BLUE = "\u001B[34m";
    public static final String PURPLE = "\u001B[35m";
    public static final String CYAN = "\u001B[36m";

    public static void print(String s, String color) {
        if(color.equals("RED")){
            System.out.println(RED + s + RESET);
        }else if(color.equals("BLUE")){
            System.out.println(BLUE + s + RESET); 
        }
        /* System.out.println(RED + "This text is red!" + RESET);
        System.out.println(GREEN + "This text is green!" + RESET);
        System.out.println(YELLOW + "This text is yellow!" + RESET);
        System.out.println(BLUE + "This text is blue!" + RESET);
        System.out.println(PURPLE + "This text is purple!" + RESET);
        System.out.println(CYAN + "This text is cyan!" + RESET);
        System.out.println("This text is default color."); */
    }
}
