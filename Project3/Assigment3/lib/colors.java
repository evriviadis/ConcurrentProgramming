public enum Color {
    BLUE(0),
    RED(1),
    NONE(2);

    private final int value;

    // Constructor to associate integer values with enum constants
    Color(int value) {
        this.value = value;
    }

    // Getter to retrieve the integer value of an enum constant
    public int getValue() {
        return value;
    }

    // Method to get the enum constant from an integer value
    public static Color fromValue(int value) {
        for (Color color : Color.values()) {
            if (color.value == value) {
                return color;
            }
        }
        throw new IllegalArgumentException("Invalid value: " + value);
    }
}
