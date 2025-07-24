// BankSim3000
//
// The purpose of this bank and teller simulation is to help a bank manager to make an informed
// decision on how many tellers to hire at a branch with longer than desired wait times.

#include <iostream>
#include <variant>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>
#include <optional>

using namespace std;

const size_t MIN_TELLERS = 1;
const size_t MAX_TELLERS = 5;

// Integer time units.
using Time = int;

// We will be tracking teller state in a variable std::vector.
using TellerIndex = size_t;

// Arrival event containing only the arrival and transaction times.
struct ArrivalEvent {
    Time arrivalTime;
    Time transactionTime;
};

// This is a common idiom in FP, wrapping a type in another to yield better
// semantics (meaning) while gaining some static type checking. This stacking can
// usually be optimized out by the compiler. It could also be a provisional
// placeholder for types that might be expanded later.
struct Customer {
    ArrivalEvent arrivalEvent;
};

// A departure event including the expected departure time and the
// teller being departed from.
struct DepartureEvent {
    Time departureTime;
    TellerIndex tellerIndex;
};

// Either an arrival or departure event. Variant can be thought of as a safer union.
using Event = std::variant<ArrivalEvent, DepartureEvent>;

// Helper function to get the time from either an arrival or departure event.
Time get_event_time(const Event& e) {
    if(holds_alternative<ArrivalEvent>(e)) {
        return get<ArrivalEvent>(e).arrivalTime;
    }
    return get<DepartureEvent>(e).departureTime;
}

// A compare functor / function object for the priority queue. Creates a min-heap.
struct CompareEvent {
    bool operator()(const Event & e1, const Event & e2) {
        // TODO: Create a min heap by replacing true with a boolean expression. Hint: Consult std::priority_queue.
        return get_event_time(e1) > get_event_time(e2);
    }
};

// Models a teller for the simulation. Holds availability and when the teller started to become busy.
// Also, automatically accumulates elapsed busy time.
//
// Hint: Use startWork and stopWork from the event processing methods to track
//       teller activity as events are processed.
class Teller {
private:
    // Optional is an ADT that either holds nothing when the teller isn't
    // busy, represented by nullopt, or a time value of when the teller
    // started working.
    optional<Time> startBusy;
    // Accumulated busy time for the teller.
    Time elapsedTimeBusy;

public:
    Teller() : startBusy(nullopt), elapsedTimeBusy(0) {}

    bool isAvailable() {
        return !startBusy.has_value();
    }

    void startWork(Time currentTime) {
        startBusy = currentTime;
    }

    void stopWork(Time currentTime) {
        // Only calculate and accumulate if the teller was actually busy
        if (startBusy.has_value()) {
            Time elapsedTime = currentTime - startBusy.value();
            elapsedTimeBusy += elapsedTime;
            startBusy = nullopt; // Teller is now available
        }
    }

    // Returns the final elapsed time a teller has been working after the simulation is finished.
    Time elapsedTimeWorking() {
        // If the simulation ends while the teller is still busy,
        // we should account for the time they were busy until the end of the simulation.
        // However, for this simulation, all events lead to a stop or next customer,
        // so `stopWork` should have been called for all busy periods.
        // If the simulation logic ensures all busy periods conclude with a departure event,
        // this method should return the final accumulated value.
        return elapsedTimeBusy;
    }
};

struct SimulationResults {
    vector<Time> elapsedTimeBusy;

    // Finds the max teller time and is perhaps proportional to the customer wait time.
    Time maxTellerBusyTime() {
        if (elapsedTimeBusy.empty()) {
            return 0;
        }
        return *max_element(elapsedTimeBusy.begin(), elapsedTimeBusy.end());
    }

    SimulationResults(vector<Time> elapsedTimeBusy) : elapsedTimeBusy(elapsedTimeBusy) { }
};

// A line of customers waiting to be served by a teller.
using BankLine = queue<Customer>;
// The event priority queue used by the simulation.
using EventQueue = priority_queue<Event, vector<Event>, CompareEvent>;
// A list of arrival events used to start the simulation.
using SimulationInput = vector<ArrivalEvent>;

class BankSim3000 {
private:
    // Input is stored locally to help restart the simulation for multiple tellers.
    SimulationInput simulationInput;
    // The event queue. Initially this is loaded with the simulation input.
    EventQueue eventQueue;
    // The bank line. Initially this is empty.
    BankLine bankLine;

    // One teller simulation state for each teller.
    vector<Teller> tellers;

    // Resets the tellers vector to the requested size and initialized to the default constructor.
    void resetTellers(size_t tellerCount) {
        if(tellerCount != tellers.size()) {
            tellers.reserve(tellerCount);
        }

        tellers.clear();

        for(size_t i=0; i<tellerCount; ++i) {
            tellers.emplace_back();
        }
    }

    // Clears the bank line.
    void clearBankLine() {
        assert(bankLine.empty()); // It should already be cleared after a complete simulation run.
        while(!bankLine.empty()) {
            bankLine.pop();
        }
    }

    // Clears the event queue and initializes it to our input data.
    void setupEventQueue() {
        assert(eventQueue.empty()); // Should also already be empty after a complete simulation.
        while(!eventQueue.empty()) {
            eventQueue.pop();
        }

        // TODO: Load all the input data from simulationInput into the event priority queue.
        for (const auto& arrival : simulationInput){
            eventQueue.push(arrival);
        }
    }

    // Sets up the simulation for the given number of tellers.
    void setupSimulation(size_t tellerCount) {
        if(tellerCount < MIN_TELLERS) {
            throw invalid_argument("Teller count must >= 1");
        }
        if(tellerCount > MAX_TELLERS) {
            throw invalid_argument("Teller count must be <= 5");
        }

        setupEventQueue();

        resetTellers(tellerCount);

        clearBankLine();
    }

    // Processes either an arrival or a departure event.
    void processEvent(Time currentTime, const Event & e) {
        if(holds_alternative<ArrivalEvent>(e)) {
            ArrivalEvent arrivalEvent = get<ArrivalEvent>(e);
            processArrival(currentTime, arrivalEvent);
        } else {
            assert(holds_alternative<DepartureEvent>(e));
            DepartureEvent departureEvent = get<DepartureEvent>(e);
            processDeparture(currentTime, departureEvent);
        }
    }

    // Helper used by processArrival.
    // Returns the index of an available teller or nullopt if all are busy.
    optional<size_t> searchAvailableTellers() {
        for(size_t i=0; i<tellers.size(); ++i) {
            if(tellers[i].isAvailable()) {
                return i;
            }
        }
        return nullopt;
    }

    // Process arrival events.
    //
    // If teller is not available or the bank line is full then we're busy,
    // place customer at the end of the bank line. Otherwise, we weren't
    // busy so start teller work and add a new departure event to the event queue.
    void processArrival(Time currentTime, const ArrivalEvent& arrivalEvent) {
        auto teller_index_opt = searchAvailableTellers();

        bool is_teller_available = teller_index_opt.has_value();

        // TODO: Process an arrival event. Don't forget to set a teller to busy if they aren't.
        if (is_teller_available) {
            TellerIndex tellerIndex = teller_index_opt.value();
            tellers[tellerIndex].startWork(currentTime);

            Time departureTime = currentTime + arrivalEvent.transactionTime;
            eventQueue.push(DepartureEvent{departureTime, tellerIndex});
        } else {
            bankLine.push(Customer{arrivalEvent});
        }
    }

    // Process departure events.
    //
    // If bank line is empty then the teller should stop working.
    // Otherwise, take the next customer off the bank line and enqueue a new departure
    // event into the event priority queue.
    void processDeparture(Time currentTime, const DepartureEvent& departureEvent) {
        size_t tellerIndex = departureEvent.tellerIndex;

        // First, mark the *current* transaction as complete and accumulate its busy time.
        // This is crucial to correctly account for the time spent on the just-finished customer.
        tellers[tellerIndex].stopWork(currentTime); // This calls stopWork to record time for completed customer

        // TODO: Process a departure event. Don't forget to set a teller to not-working if they are.
        if (bankLine.empty()) {
            // This is a NO-OP
         
        } else {
         
            Customer nextCustomer = bankLine.front();
            bankLine.pop();

            // Teller starts working on new customer
            tellers[tellerIndex].startWork(currentTime); 

           
            Time newDepartureTime = currentTime + nextCustomer.arrivalEvent.transactionTime;
            eventQueue.push(DepartureEvent{newDepartureTime, tellerIndex});
        }
    }

    // Runs the simulation.
    void runSimulation() {
        while(!eventQueue.empty()) {
            // Remove event.
            Event e = eventQueue.top();
            eventQueue.pop();

            processEvent(get_event_time(e), e);
        }
    }

    SimulationResults gatherResults() {
        // Transform is like map in more functional languages. It takes an input vector and fills
        // a new vector with the results of the given lambda function passed as a parameter.
        vector<Time> elapsedTimeBusy(tellers.size());
        transform(tellers.begin(), tellers.end(), elapsedTimeBusy.begin(), [](auto& teller) {
            return teller.elapsedTimeWorking();
        });

        return SimulationResults {elapsedTimeBusy};
    }

public:

    BankSim3000(SimulationInput simulationInput) : simulationInput(simulationInput) { }

    Time maxTellerBusyTime(size_t tellerCount) {
        setupSimulation(tellerCount);

        runSimulation();

        return gatherResults().maxTellerBusyTime();
    }
};

int main() {
    // Do not change the input.
    SimulationInput SimulationInput00 = {{20, 6}, {22, 4}, {23, 2}, {30, 3}};

    BankSim3000 bankSim(SimulationInput00);

    cout << "Time waiting with 1 teller: " << bankSim.maxTellerBusyTime(1) << endl;
    cout << "Time waiting with 2 tellers: " << bankSim.maxTellerBusyTime(2) << endl;
    cout << "Time waiting with 3 tellers: " << bankSim.maxTellerBusyTime(3) << endl;
    cout << "Time waiting with 4 tellers: " << bankSim.maxTellerBusyTime(4) << endl;
    cout << "Time waiting with 5 tellers: " << bankSim.maxTellerBusyTime(5) << endl;

    return 0;
}