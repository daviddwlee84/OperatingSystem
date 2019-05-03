# The Reading Room Problem using Monitor

> It is a resource management problem

## Scenario

* There is a reading room with 50 seats.
* Reader must register on a register sheet when they come in.
* One row entry means a seat. Including a seat number and a reader's name.
* Reader must remove the register information when they are leaving.

## Key Point - Synchronization and Mutual Exclusion

Resources

* Register Sheet <-- used to protect the seats
* Seats

Synchronization

* Multiple readers may have want to enter the reading room at the same time

Mutual Exclusion

* Only one reader can modify the register sheet

## Solution

### Using Semaphore

Use 2 semaphore

1. Semaphore for seat. The initial value is the number of seats. (50 in this case)
2. Semaphore for register sheet. The initial value is 1. (like a mutex)

The following is a pseudocode of a reader thread. (Assume 50 student with their own id)

```cpp
#define MAX_SEAT 50

Semaphore seats(MAX_SEAT);
Semaphore sheet(1);

bool register_sheet[MAX_SEAT] = {0};

void reader_thread(int reader_num){
    seats.P();
    sheet.P();
    // Register on the sheet //
    register_sheet[reader_num] = true;
    ///////////////////////////
    sheet.V();

    // Reading in the reading room

    sheet.P();
    // Remove the registration off the sheet //
    register_sheet[reader_num] = false;
    ///////////////////////////////////////////
    sheet.V();
    seats.V();
}
```

### Using Monitor - simulate in C++ like pseudocode

Create a monitor with 2 operation. `enter_room()` and `exit_room()`.
Use a condition variable to solve the synchronization problem.
Use a mutex to protect the register sheet.

The following is a pseudocode of a monitor.

```cpp
#define MAX_SEAT 50

class ReadingRoom // This is a monitor
{
  public:
    // will return the seat number to reader thread
    int enter_room(void)
    {
        if (seats == 0) // if no seats, go to sleep
            cond_var.wait();

        register.lock();
        for (int i = 0; i < MAX_SEAT; i++) {
            if (!register_sheet[i]) {
                empty_seat_num = i;
                break;
            }
        }
        register_sheet[empty_seat_num] = true;
        seat -= 1;
        register.unlock();
        return empty_seat_num;
    };

    // clear the reader seat number on the register sheet
    void exit_room(int seat_num)
    {
        register.lock();
        register_sheet[seat_num] = false;
        seat += 1;
        register.unlock();
        cond_var.signal(); // notify next reader to come in if any
    };
  private:
    bool register_sheet[MAX_SEAT] = {0};
    int seats = MAX_SEAT;
    Mutex register;
    CondVar cond_var;
}
```

The following is a pseudocode of a reader thread.

```cpp
ReadingRoom reading_room_monitor;

void reader_thread(void) {
    int reader_seat_number = reading_room_monitor.enter_room();

    // Reading in the reading room

    reading_room_monitor.exit_room(reader_seat_number);
}
```

### Using Monitor - assume the operations of the Monitor are mutual exclusive (like Java)

> Thus we don't need `Mutex register` to protect the register_sheet

The following is a pseudocode of a monitor.

```java
public static final int MAX_SEAT = 50;

public class ReadingRoom { // This is a monitor
    private seats;
    private boolean[] register_sheet;

    public ReadingRoom(void) { // Constructor: Initialization
        seats = MAX_SEAT;
        register_shee = new boolean[MAX_SEAT];
        for (int i = 0; i < MAX_SEAT; i++) {
            register_sheet[i] = false;
        }
    }

    // will return the seat number to reader thread
    public synchronized int enter_room(void) {
        if (seats == 0) // if no seats, go to sleep
            wait();

        // find an empty seat
        for (int i = 0; i < MAX_SEAT; i++) {
            if (!register_sheet[i]) {
                empty_seat_num = i;
                break;
            }
        }
        seat -= 1;

        return empty_seat_num;
    }

    // clear the reader seat number on the register sheet
    public synchronized void exit_room(int seat_num) {
        register_sheet[seat_num] = false;
        seat += 1;

        notify(); // notify next reader to come in if any
    }
}
```

The following is a pseudocode of a reader thread.

```java
ReadingRoom reading_room = new ReadingRoom();

class Reader extends Thread {
    private int reader_seat_number;

    public void run() {
        reader_seat_number = reading_room.enter_room()

        // Reading in the reading room

        reading_room.exit_room(reader_seat_number)
    }
}
```

## Problems and Answer

1. To describe the reader, how many program do you need, and how many process do you need

    It is able to have more than 50 reader threads.

2. Try to describe the relationship between reader processes using P, V operation
    * P: when they want to sit on a seat
    * V: when they leave a seat
3. Where is the Deadlock most likely to appear

    If someone is going to register the seat but leave (been interrupted, like taking off the pencil).
    Therefore, no one can adjust the sheet.
    Or maybe leaving the reading room but forgot to inform next one to come in.

## Links

* [用信號量解決進程的同步與互斥探討 - 問題6：閱覽室問題 / 圖書館問題](http://whatbeg.com/2017/03/06/semaphore.html#%E9%97%AE%E9%A2%986%EF%BC%9A%E9%98%85%E8%A7%88%E5%AE%A4%E9%97%AE%E9%A2%98-%E5%9B%BE%E4%B9%A6%E9%A6%86%E9%97%AE%E9%A2%98)
