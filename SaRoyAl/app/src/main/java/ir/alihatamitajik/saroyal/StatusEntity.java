package ir.alihatamitajik.saroyal;

public class StatusEntity {
    String Current;
    int temprature;
    int battery;

    public String getCurrent() {
        return Current;
    }

    public void setCurrent(String current) {
        Current = current;
    }

    public int getTemprature() {
        return temprature;
    }

    public void setTemprature(int temprature) {
        this.temprature = temprature;
    }

    public int getBattery() {
        return battery;
    }

    public void setBattery(int battery) {
        this.battery = battery;
    }
}
