#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <string>

struct PrisonerState {
    int count_visits = 0;
    bool turned_on_once = false;
};

struct Simulation {
    std::vector<PrisonerState> prisoners = std::vector<PrisonerState>(20);
    enum { ON, OFF } lamp = OFF;
    int leader = 0; // prisoner 0 is the counter/leader
    int leader_count = 0; // counts how many non-leaders have been seen via lamp
};

int pick_random_index(int n = 20) {
    return std::rand() % n;
}

void handle_visit(Simulation &sim, int idx) {
    if (idx == sim.leader) {
        if (sim.lamp == Simulation::ON) {
            sim.lamp = Simulation::OFF;
            sim.leader_count++;
        }
    } else {
        if (!sim.prisoners[idx].turned_on_once && sim.lamp == Simulation::OFF) {
            sim.lamp = Simulation::ON;
            sim.prisoners[idx].turned_on_once = true;
        }
    }
    sim.prisoners[idx].count_visits++;
}

bool should_stop(const Simulation &sim) {
    return sim.leader_count >= 19;
}

void print_step(const Simulation &sim, int idx) {
    std::cout << "idx=" << idx
              << " visits=" << sim.prisoners[idx].count_visits
              << " lamp=" << (sim.lamp == Simulation::ON ? "ON" : "OFF")
              << " leader_count=" << sim.leader_count << std::endl;
}

void print_per_prisoner(const Simulation &sim) {
    for (int i = 0; i < 20; ++i) {
        std::cout << "P" << i
                  << ": visits=" << sim.prisoners[i].count_visits
                  << ", turned_on_once=" << (sim.prisoners[i].turned_on_once ? 1 : 0)
                  << std::endl;
    }
}

long long print_aligned_counts(const Simulation &sim) {
    int max_visits = 0;
    for (int i = 0; i < 20; ++i)
        if (sim.prisoners[i].count_visits > max_visits) max_visits = sim.prisoners[i].count_visits;
    int width = std::max((int)std::to_string(19).size(), (int)std::to_string(max_visits).size());
    width = std::max(width, 2);

    // std::cout << "Indices      : ";
    // for (int i = 0; i < 20; ++i)
    //     std::cout << std::setw(width) << i << ' ';
    // std::cout << " | sum\n";

    long long total_visits = 0;
    // std::cout << "Visits counts: ";
    for (int i = 0; i < 20; ++i) {
        // std::cout << std::setw(width) << sim.prisoners[i].count_visits << ' ';
        total_visits += sim.prisoners[i].count_visits;
    }
    // std::cout <<"\n";
    // std::cout << " | " << total_visits << std::endl;
    return total_visits;
}

long long print_summary(const Simulation &sim) {
    // std::cout << "Leader counted all other prisoners (19)." << std::endl;
    // print_per_prisoner(sim);
    return print_aligned_counts(sim);
}

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    long long max_steps = 0;
    long long tmp = 0;
    int sim_num = 100000;
    for (int i = 0 ; i < sim_num; ++i)
    {
        Simulation sim;
        while (true) {
            int idx = pick_random_index();
            handle_visit(sim, idx);
            //print_step(sim, idx);
            if (should_stop(sim)) {
                tmp = print_summary(sim);
                max_steps = std::max(max_steps, tmp);
                tmp = 0;
                // std::cout << "current max steps: " << max_steps << std::endl;
                break;
            }
        }
    }
    std::cout << "Max steps in "<< sim_num << " simulations: " << max_steps << std::endl;
    return 0;
}
