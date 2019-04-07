# Process Synchronization and Interprocess Communication

## Overview

### Process Parallelism

#### Comparison between sequential system and parallel system

## Race Condition

## Critical Section/Region

### Solution of Mutual Exclusion

#### Software approach

* Dekker
* Peterson

#### Hardware approach

* Block Interrupt
* TSL(XCHG) Instruction

## Semaphore

### P, V Operation

#### P

#### V

## Monitor

### Wait and Signal Operation

### MESA Monitor

## Lock and Condition Variable

## Implementation

* Application
  * Parallel process
* System
  * Synchronization Mechanism
    * Semaphore
    * Monitor
    * Lock
    * Condition Variable
* Hardware
  * Atomic Operation
    * Load/Store
    * Enable/Disable Interrupt
    * Test & Set
    * ...

## Interprocess Communication (IPC)

## Resources

### Book

* Ch3 Processes
  * Ch3.4 Interprocess Communication
    * Ch3.4.1 Shared-Memory Systems
* Ch5 Process Synchronization
  * Ch5.2 The Critical-Section Problem
  * Ch5.3 Peterson's Solution
  * Ch5.4 Synchronization Hardware
  * Ch5.5 Mutex Locks
  * Ch5.6 Semaphores
  * Ch5.7 Classic Problems of Synchronization
    * Ch5.7.1 The Bounded-Buffer Problem
    * Ch5.7.2 The Readers-Writers Problem
    * Ch5.7.3 The Dining-Philosophers Problem
  * Ch5.8 Monitors
  * Ch5.9 Synchronization Examples
    * Ch5.9.2 Synchronization in Linux

Modern Operating Systems 4ed.

* Ch2.3 Interprocess Communication
  * Ch2.3.1 Race Conditinos
  * Ch2.3.2 Critical Regions
  * Ch2.3.3 Mutual Exclusion with Busy Waiting
  * Ch2.3.4 Sleep and Wakeup
  * Ch2.3.5 Semaphores
  * Ch2.3.6 Mutexes
  * Ch2.3.7 Monitors 管程
  * Ch2.3.8 Message Passing
  * Ch2.3.9 Barriers
