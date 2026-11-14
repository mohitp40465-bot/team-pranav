/**
 * SmartRide - Vehicle Rental & Fleet Management
 * Clean, modern client application
 */

// ==========================================
// 1. DATA MODELS
// ==========================================

class Vehicle {
    constructor(id, name, type, brand, rentPerDay, isAvailable = true) {
        this.id = id;
        this.name = name;
        this.type = type; // "Car", "Bike", "Scooter"
        this.brand = brand;
        this.rentPerDay = parseFloat(rentPerDay);
        this.isAvailable = Boolean(isAvailable);
    }

    calculateRent(days) {
        if (days <= 0) return 0;
        return this.rentPerDay * days;
    }
}

class Car extends Vehicle {
    constructor(id, name, brand, rentPerDay, seats, fuelType, isAvailable = true) {
        super(id, name, "Car", brand, rentPerDay, isAvailable);
        this.seats = parseInt(seats);
        this.fuelType = fuelType; // "Petrol", "Diesel", "Electric", "CNG"
    }

    calculateRent(days) {
        let base = super.calculateRent(days);
        // 5% discount for 7+ days rental
        if (days >= 7) {
            base *= 0.95;
        }
        return base;
    }
}

class Bike extends Vehicle {
    constructor(id, name, brand, rentPerDay, engineCc, category, isAvailable = true) {
        super(id, name, "Bike", brand, rentPerDay, isAvailable);
        this.engineCc = parseInt(engineCc);
        this.category = category; // "Sport", "Cruiser", "Commuter"
    }

    calculateRent(days) {
        let base = super.calculateRent(days);
        // Nominal helmet & gear fee for Sport / Cruiser
        if (this.category === "Sport" || this.category === "Cruiser") {
            base += 50.0;
        }
        return base;
    }
}

class Scooter extends Vehicle {
    constructor(id, name, brand, rentPerDay, storageLitres, isElectric, isAvailable = true) {
        super(id, name, "Scooter", brand, rentPerDay, isAvailable);
        this.storageLitres = parseFloat(storageLitres);
        this.isElectric = Boolean(isElectric);
    }

    calculateRent(days) {
        let base = super.calculateRent(days);
        // 5% Green EV Subsidy
        if (this.isElectric) {
            base *= 0.95;
        }
        return base;
    }
}

class Customer {
    constructor(id, name, phone) {
        this.id = id;
        this.name = name;
        this.phone = phone;
    }
}

class Booking {
    constructor(bookingId, customerId, customerName, vehicleId, vehicleName, days, totalRent, status = "Active") {
        this.bookingId = bookingId;
        this.customerId = customerId;
        this.customerName = customerName;
        this.vehicleId = vehicleId;
        this.vehicleName = vehicleName;
        this.rentalDays = parseInt(days);
        this.totalRent = parseFloat(totalRent);
        this.status = status;
    }
}

// ==========================================
// 2. WAITLIST & TRANSACTION HISTORY
// ==========================================

class QueueNode {
    constructor(customerId, customerName, requestedVehicleId) {
        this.customerId = customerId;
        this.customerName = customerName;
        this.requestedVehicleId = requestedVehicleId;
        this.next = null;
    }
}

class WaitingQueue {
    constructor() {
        this.front = null;
        this.rear = null;
        this.count = 0;
    }

    enqueue(cId, cName, vId) {
        const newNode = new QueueNode(cId, cName, vId);
        if (!this.rear) {
            this.front = this.rear = newNode;
        } else {
            this.rear.next = newNode;
            this.rear = newNode;
        }
        this.count++;
    }

    dequeue() {
        if (!this.front) return null;
        const temp = this.front;
        this.front = this.front.next;
        if (!this.front) this.rear = null;
        this.count--;
        return temp;
    }

    hasCustomerForVehicle(vehicleId) {
        let curr = this.front;
        while (curr) {
            if (curr.requestedVehicleId === vehicleId) return true;
            curr = curr.next;
        }
        return false;
    }

    serveCustomerForVehicle(vehicleId) {
        let curr = this.front;
        let prev = null;
        while (curr) {
            if (curr.requestedVehicleId === vehicleId) {
                if (curr === this.front) {
                    this.front = this.front.next;
                    if (!this.front) this.rear = null;
                } else {
                    prev.next = curr.next;
                    if (curr === this.rear) this.rear = prev;
                }
                this.count--;
                return curr;
            }
            prev = curr;
            curr = curr.next;
        }
        return null;
    }

    toArray() {
        const arr = [];
        let curr = this.front;
        while (curr) {
            arr.push({
                customerId: curr.customerId,
                customerName: curr.customerName,
                requestedVehicleId: curr.requestedVehicleId
            });
            curr = curr.next;
        }
        return arr;
    }

    loadFromArray(arr) {
        this.front = this.rear = null;
        this.count = 0;
        if (Array.isArray(arr)) {
            arr.forEach(item => this.enqueue(item.customerId, item.customerName, item.requestedVehicleId));
        }
    }
}

class HistoryNode {
    constructor(booking) {
        this.bookingId = booking.bookingId;
        this.customerId = booking.customerId;
        this.customerName = booking.customerName;
        this.vehicleId = booking.vehicleId;
        this.vehicleName = booking.vehicleName;
        this.rentalDays = booking.rentalDays;
        this.totalRent = booking.totalRent;
        this.status = booking.status;
        this.next = null;
    }
}

class RentalHistory {
    constructor() {
        this.head = null;
        this.tail = null;
        this.recordCount = 0;
    }

    insertRecord(booking) {
        const newNode = new HistoryNode(booking);
        if (!this.head) {
            this.head = this.tail = newNode;
        } else {
            this.tail.next = newNode;
            this.tail = newNode;
        }
        this.recordCount++;
    }

    updateStatus(bookingId, newStatus, finalRent) {
        let curr = this.head;
        while (curr) {
            if (curr.bookingId === bookingId) {
                curr.status = newStatus;
                if (finalRent !== undefined) curr.totalRent = finalRent;
                return true;
            }
            curr = curr.next;
        }
        return false;
    }

    calculateTotalRevenue() {
        let total = 0;
        let curr = this.head;
        while (curr) {
            if (curr.status === "Completed") {
                total += curr.totalRent;
            }
            curr = curr.next;
        }
        return total;
    }

    countCompleted() {
        let c = 0;
        let curr = this.head;
        while (curr) {
            if (curr.status === "Completed") c++;
            curr = curr.next;
        }
        return c;
    }

    toArray() {
        const arr = [];
        let curr = this.head;
        while (curr) {
            arr.push({
                bookingId: curr.bookingId,
                customerId: curr.customerId,
                customerName: curr.customerName,
                vehicleId: curr.vehicleId,
                vehicleName: curr.vehicleName,
                rentalDays: curr.rentalDays,
                totalRent: curr.totalRent,
                status: curr.status
            });
            curr = curr.next;
        }
        return arr;
    }

    loadFromArray(arr) {
        this.head = this.tail = null;
        this.recordCount = 0;
        if (Array.isArray(arr)) {
            arr.forEach(item => this.insertRecord(item));
        }
    }
}

// ==========================================
// 3. APPLICATION STATE & STORAGE
// ==========================================

let vehicles = [];
let customers = [];
let activeBookings = [];
const waitingQueue = new WaitingQueue();
const rentalHistory = new RentalHistory();

let selectedVehicleForBooking = null;
let selectedBookingForReturn = null;
let currentVehicleFilter = "all";

// Default Seed Fleet
function getSampleVehicles() {
    return [
        new Car("C101", "Honda City", "Honda", 2200, 5, "Petrol", true),
        new Car("C102", "Hyundai Creta", "Hyundai", 2800, 5, "Diesel", true),
        new Car("C103", "Tata Nexon EV", "Tata", 2500, 5, "Electric", true),
        new Car("C104", "Maruti Swift", "Maruti", 1600, 5, "Petrol", true),
        new Bike("B201", "Royal Enfield 350", "Royal Enfield", 1200, 350, "Cruiser", true),
        new Bike("B202", "Yamaha R15 V4", "Yamaha", 1000, 155, "Sport", true),
        new Bike("B203", "Hero Splendor+", "Hero", 500, 100, "Commuter", true),
        new Bike("B204", "KTM Duke 250", "KTM", 1400, 248, "Sport", true),
        new Scooter("S301", "Honda Activa 6G", "Honda", 600, 18, false, true),
        new Scooter("S302", "Ather 450X", "Ather", 800, 22, true, true),
        new Scooter("S303", "TVS Jupiter 125", "TVS", 550, 33, false, true),
        new Scooter("S304", "Ola S1 Pro", "Ola", 750, 34, true, true)
    ];
}

function getSampleCustomers() {
    return [
        new Customer("C101", "Aarav Sharma", "9876543210"),
        new Customer("C102", "Priya Verma", "9123456780"),
        new Customer("C103", "Rohan Mehta", "9988776655"),
        new Customer("C104", "Ananya Iyer", "9811223344"),
        new Customer("C105", "Kabir Patel", "9765432109")
    ];
}

function getSampleHistory() {
    return [
        { bookingId: "BK1000", customerId: "C101", customerName: "Aarav Sharma", vehicleId: "C101", vehicleName: "Honda City", rentalDays: 3, totalRent: 6600.0, status: "Completed" },
        { bookingId: "BK1001", customerId: "C102", customerName: "Priya Verma", vehicleId: "B202", vehicleName: "Yamaha R15 V4", rentalDays: 2, totalRent: 2050.0, status: "Completed" },
        { bookingId: "BK1002", customerId: "C103", customerName: "Rohan Mehta", vehicleId: "S302", vehicleName: "Ather 450X", rentalDays: 4, totalRent: 3040.0, status: "Completed" }
    ];
}

function saveState() {
    localStorage.setItem("smartride_vehicles", JSON.stringify(vehicles));
    localStorage.setItem("smartride_customers", JSON.stringify(customers));
    localStorage.setItem("smartride_active_bookings", JSON.stringify(activeBookings));
    localStorage.setItem("smartride_waiting_queue", JSON.stringify(waitingQueue.toArray()));
    localStorage.setItem("smartride_rental_history", JSON.stringify(rentalHistory.toArray()));
}

function loadState() {
    const vData = localStorage.getItem("smartride_vehicles");
    const cData = localStorage.getItem("smartride_customers");
    const bData = localStorage.getItem("smartride_active_bookings");
    const qData = localStorage.getItem("smartride_waiting_queue");
    const hData = localStorage.getItem("smartride_rental_history");

    if (vData && cData) {
        try {
            const rawV = JSON.parse(vData);
            vehicles = rawV.map(v => {
                if (v.type === "Car") return new Car(v.id, v.name, v.brand, v.rentPerDay, v.seats, v.fuelType, v.isAvailable);
                if (v.type === "Bike") return new Bike(v.id, v.name, v.brand, v.rentPerDay, v.engineCc, v.category, v.isAvailable);
                if (v.type === "Scooter") return new Scooter(v.id, v.name, v.brand, v.rentPerDay, v.storageLitres, v.isElectric, v.isAvailable);
                return new Vehicle(v.id, v.name, v.type, v.brand, v.rentPerDay, v.isAvailable);
            });

            customers = JSON.parse(cData).map(c => new Customer(c.id, c.name, c.phone));
            activeBookings = bData ? JSON.parse(bData).map(b => new Booking(b.bookingId, b.customerId, b.customerName, b.vehicleId, b.vehicleName, b.rentalDays, b.totalRent, b.status)) : [];
            waitingQueue.loadFromArray(qData ? JSON.parse(qData) : []);
            rentalHistory.loadFromArray(hData ? JSON.parse(hData) : []);
            return;
        } catch (e) {
            console.error("Resetting state to clean defaults", e);
        }
    }

    resetToSampleData(false);
}

function resetToSampleData(showToastMsg = true) {
    vehicles = getSampleVehicles();
    customers = getSampleCustomers();
    activeBookings = [];
    waitingQueue.loadFromArray([]);
    rentalHistory.loadFromArray(getSampleHistory());
    saveState();
    refreshAllViews();
    if (showToastMsg) showToast("Demo fleet data loaded successfully!", "success");
}

// ==========================================
// 4. UI NAVIGATION & RENDERING
// ==========================================

function switchTab(tabId) {
    document.querySelectorAll(".tab-pane").forEach(el => el.classList.add("hidden"));
    document.querySelectorAll(".nav-tab").forEach(el => el.classList.remove("active-tab"));

    const targetTab = document.getElementById(`tab-${tabId}`);
    const targetNav = document.getElementById(`nav-${tabId}`);
    if (targetTab) targetTab.classList.remove("hidden");
    if (targetNav) targetNav.classList.add("active-tab");
}

function refreshAllViews() {
    renderDashboard();
    renderVehicles();
    renderCustomers();
    renderActiveRentals();
    renderQueue();
    renderHistory();
}

// Dashboard
function renderDashboard() {
    const totalV = vehicles.length;
    const availableV = vehicles.filter(v => v.isAvailable).length;
    const rentedV = totalV - availableV;
    const totalRev = rentalHistory.calculateTotalRevenue();
    const activeB = activeBookings.length;
    const completedB = rentalHistory.countCompleted();
    const queueCount = waitingQueue.count;

    document.getElementById("stat-total-vehicles").innerText = totalV;
    document.getElementById("stat-available-vehicles").innerText = `${availableV} Available`;
    document.getElementById("stat-rented-vehicles").innerText = `${rentedV} Rented`;
    document.getElementById("stat-total-revenue").innerHTML = `&#8377;${totalRev.toFixed(2)}`;
    document.getElementById("stat-active-rentals").innerText = activeB;
    document.getElementById("stat-completed-rentals").innerText = completedB;
    document.getElementById("stat-queue-count").innerText = queueCount;

    // Queue badge in navbar
    const badge = document.getElementById("queue-badge");
    if (queueCount > 0) {
        badge.innerText = queueCount;
        badge.classList.remove("hidden");
    } else {
        badge.classList.add("hidden");
    }

    // Category breakdown
    const cars = vehicles.filter(v => v.type === "Car").length;
    const bikes = vehicles.filter(v => v.type === "Bike").length;
    const scooters = vehicles.filter(v => v.type === "Scooter").length;

    document.getElementById("count-cars").innerText = cars;
    document.getElementById("count-bikes").innerText = bikes;
    document.getElementById("count-scooters").innerText = scooters;

    document.getElementById("bar-cars").style.width = totalV ? `${(cars / totalV) * 100}%` : "0%";
    document.getElementById("bar-bikes").style.width = totalV ? `${(bikes / totalV) * 100}%` : "0%";
    document.getElementById("bar-scooters").style.width = totalV ? `${(scooters / totalV) * 100}%` : "0%";

    // Recent Active Rentals
    const container = document.getElementById("dashboard-active-rentals");
    if (activeBookings.length === 0) {
        container.innerHTML = `<p class="text-xs text-slate-500 italic py-2">No active rentals right now. All fleet vehicles are ready to book.</p>`;
    } else {
        container.innerHTML = activeBookings.slice(0, 3).map(b => `
            <div class="flex items-center justify-between p-3 rounded-xl bg-slate-900/60 border border-slate-700/50">
                <div class="flex items-center gap-3">
                    <div class="w-8 h-8 rounded-lg bg-cyan-500/10 text-cyan-400 flex items-center justify-center font-bold text-xs">
                        ${b.bookingId}
                    </div>
                    <div>
                        <h4 class="text-xs font-semibold text-white">${b.vehicleName} <span class="text-slate-400 font-normal">(${b.vehicleId})</span></h4>
                        <p class="text-[11px] text-slate-400">Rented by: <span class="text-indigo-300 font-medium">${b.customerName}</span> &bull; ${b.rentalDays} days</p>
                    </div>
                </div>
                <div class="flex items-center gap-3">
                    <span class="text-xs font-bold text-emerald-400">&#8377;${b.totalRent.toFixed(2)}</span>
                    <button onclick="openReturnModal('${b.bookingId}')" class="px-2.5 py-1 rounded-lg bg-cyan-600 hover:bg-cyan-500 text-white text-xs font-medium transition">
                        Return
                    </button>
                </div>
            </div>
        `).join("");
    }
}

// Fleet Vehicles
function setVehicleFilter(filter) {
    currentVehicleFilter = filter;
    document.querySelectorAll(".filter-btn").forEach(btn => {
        if (btn.getAttribute("data-filter") === filter) {
            btn.classList.add("active-filter");
            btn.classList.remove("text-slate-400");
        } else {
            btn.classList.remove("active-filter");
            btn.classList.add("text-slate-400");
        }
    });
    renderVehicles();
}

function renderVehicles() {
    const grid = document.getElementById("vehicles-grid");
    const search = document.getElementById("vehicle-search-input").value.toLowerCase().trim();

    let filtered = vehicles.filter(v => {
        const matchesSearch = v.id.toLowerCase().includes(search) ||
                              v.name.toLowerCase().includes(search) ||
                              v.brand.toLowerCase().includes(search);

        if (!matchesSearch) return false;

        if (currentVehicleFilter === "all") return true;
        if (currentVehicleFilter === "available") return v.isAvailable;
        return v.type === currentVehicleFilter;
    });

    if (filtered.length === 0) {
        grid.innerHTML = `
            <div class="col-span-full py-12 text-center text-slate-500">
                <i class="fa-solid fa-car-tunnel text-3xl mb-2 text-slate-600"></i>
                <p class="text-xs">No vehicles found matching the selected filter or search query.</p>
            </div>
        `;
        return;
    }

    grid.innerHTML = filtered.map(v => {
        let badgeColor = "bg-indigo-500/10 text-indigo-400 border-indigo-500/20";
        let icon = "fa-car";
        let spec = "";

        if (v.type === "Car") {
            spec = `<i class="fa-solid fa-users text-slate-400 mr-1"></i> ${v.seats} Seats &bull; <i class="fa-solid fa-gas-pump text-slate-400 mr-1"></i> ${v.fuelType}`;
        } else if (v.type === "Bike") {
            badgeColor = "bg-cyan-500/10 text-cyan-400 border-cyan-500/20";
            icon = "fa-motorcycle";
            spec = `<i class="fa-solid fa-gauge text-slate-400 mr-1"></i> ${v.engineCc}cc &bull; ${v.category}`;
        } else if (v.type === "Scooter") {
            badgeColor = "bg-emerald-500/10 text-emerald-400 border-emerald-500/20";
            icon = "fa-bolt";
            spec = `<i class="fa-solid fa-box text-slate-400 mr-1"></i> ${v.storageLitres}L Boot &bull; ${v.isElectric ? '<span class="text-emerald-400 font-semibold">Electric EV</span>' : 'Petrol'}`;
        }

        const statusBadge = v.isAvailable 
            ? `<span class="px-2 py-0.5 rounded-full text-[10px] font-bold bg-emerald-500/10 text-emerald-400 border border-emerald-500/20">Available</span>`
            : `<span class="px-2 py-0.5 rounded-full text-[10px] font-bold bg-rose-500/10 text-rose-400 border border-rose-500/20">Rented</span>`;

        const actionButton = v.isAvailable
            ? `<button onclick="openBookModal('${v.id}')" class="w-full py-2 rounded-xl bg-indigo-600 hover:bg-indigo-500 text-white font-semibold text-xs transition shadow-md shadow-indigo-600/30 flex items-center justify-center gap-1.5">
                 <i class="fa-solid fa-key"></i> Book Vehicle
               </button>`
            : `<button onclick="promptJoinQueue('${v.id}')" class="w-full py-2 rounded-xl bg-rose-600/30 hover:bg-rose-600/50 text-rose-300 font-semibold text-xs border border-rose-500/30 transition flex items-center justify-center gap-1.5">
                 <i class="fa-solid fa-hourglass-start"></i> Join Waitlist
               </button>`;

        return `
            <div class="vehicle-card bg-slate-800/60 border border-slate-700/60 rounded-2xl p-4 flex flex-col justify-between space-y-4">
                <div class="space-y-3">
                    <div class="flex justify-between items-start">
                        <span class="px-2.5 py-1 rounded-lg text-xs font-semibold border ${badgeColor} flex items-center gap-1.5">
                            <i class="fa-solid ${icon}"></i> ${v.type}
                        </span>
                        <div class="flex items-center gap-2">
                            <span class="text-xs font-mono text-slate-400">${v.id}</span>
                            ${statusBadge}
                        </div>
                    </div>

                    <div>
                        <h4 class="text-sm font-bold text-white">${v.name}</h4>
                        <p class="text-xs text-slate-400">${v.brand}</p>
                    </div>

                    <div class="text-[11px] text-slate-300 p-2 rounded-xl bg-slate-900/60 border border-slate-700/50">
                        ${spec}
                    </div>
                </div>

                <div class="space-y-3 pt-2 border-t border-slate-700/50">
                    <div class="flex justify-between items-baseline">
                        <span class="text-xs text-slate-400">Daily Rate</span>
                        <span class="text-base font-extrabold text-emerald-400">&#8377;${v.rentPerDay.toFixed(2)}<span class="text-xs font-normal text-slate-400">/day</span></span>
                    </div>
                    ${actionButton}
                </div>
            </div>
        `;
    }).join("");
}

// Customers
function renderCustomers() {
    const tbody = document.getElementById("customers-table-body");
    if (customers.length === 0) {
        tbody.innerHTML = `<tr><td colspan="4" class="text-center py-6 text-slate-500">No registered customers yet.</td></tr>`;
        return;
    }

    tbody.innerHTML = customers.map(c => `
        <tr class="hover:bg-slate-800/40 transition">
            <td class="px-4 py-3 font-mono font-semibold text-indigo-400">${c.id}</td>
            <td class="px-4 py-3 font-medium text-white">${c.name}</td>
            <td class="px-4 py-3 font-mono">${c.phone}</td>
            <td class="px-4 py-3 text-right">
                <button onclick="prefillBookingForCustomer('${c.id}')" class="px-3 py-1 rounded-lg bg-indigo-600/20 hover:bg-indigo-600/40 text-indigo-300 font-medium text-[11px] transition">
                    Book for Customer
                </button>
            </td>
        </tr>
    `).join("");
}

// Active Rentals
function renderActiveRentals() {
    const tbody = document.getElementById("active-rentals-table-body");
    if (activeBookings.length === 0) {
        tbody.innerHTML = `<tr><td colspan="7" class="text-center py-8 text-slate-500">No active vehicle rentals at this moment.</td></tr>`;
        return;
    }

    tbody.innerHTML = activeBookings.map(b => `
        <tr class="hover:bg-slate-800/40 transition">
            <td class="px-4 py-3 font-mono font-bold text-cyan-400">${b.bookingId}</td>
            <td class="px-4 py-3 font-medium text-white">${b.customerName} <span class="text-slate-400 text-[11px]">(${b.customerId})</span></td>
            <td class="px-4 py-3">${b.vehicleName} <span class="text-slate-400 text-[11px]">(${b.vehicleId})</span></td>
            <td class="px-4 py-3 font-mono">${b.rentalDays} days</td>
            <td class="px-4 py-3 font-bold text-emerald-400">&#8377;${b.totalRent.toFixed(2)}</td>
            <td class="px-4 py-3"><span class="px-2 py-0.5 rounded-full text-[10px] font-bold bg-amber-500/10 text-amber-400 border border-amber-500/20">Active</span></td>
            <td class="px-4 py-3 text-right">
                <button onclick="openReturnModal('${b.bookingId}')" class="px-3 py-1.5 rounded-lg bg-cyan-600 hover:bg-cyan-500 text-white font-semibold text-xs transition shadow-md shadow-cyan-600/30">
                    Return Vehicle
                </button>
            </td>
        </tr>
    `).join("");
}

// Waitlist
function renderQueue() {
    const list = document.getElementById("queue-list");
    const emptyState = document.getElementById("queue-empty-state");
    const countHeader = document.getElementById("queue-header-count");

    countHeader.innerText = `${waitingQueue.count} Waiting`;

    if (waitingQueue.count === 0) {
        list.innerHTML = "";
        emptyState.classList.remove("hidden");
        return;
    }

    emptyState.classList.add("hidden");
    const queueItems = waitingQueue.toArray();

    list.innerHTML = queueItems.map((item, index) => `
        <div class="queue-node flex items-center justify-between p-4 rounded-xl bg-slate-800/60 border border-slate-700/60 hover:border-rose-500/40 transition">
            <div class="flex items-center gap-3">
                <div class="w-8 h-8 rounded-lg ${index === 0 ? 'bg-rose-500 text-white shadow-lg shadow-rose-500/30' : 'bg-slate-700 text-slate-300'} flex items-center justify-center font-bold text-xs">
                    #${index + 1}
                </div>
                <div>
                    <h4 class="text-xs font-bold text-white">${item.customerName} <span class="text-slate-400 font-normal">(${item.customerId})</span></h4>
                    <p class="text-[11px] text-slate-400">Waiting for Vehicle: <span class="text-rose-400 font-mono font-semibold">${item.requestedVehicleId}</span></p>
                </div>
            </div>
            <div class="flex items-center gap-2">
                <span class="text-[11px] px-2.5 py-1 rounded-lg bg-slate-900/80 text-rose-300 border border-rose-500/20 font-medium">Priority #${index + 1}</span>
            </div>
        </div>
    `).join("");
}

// Rental History
function renderHistory() {
    const tbody = document.getElementById("history-table-body");
    const search = document.getElementById("history-search-input") ? document.getElementById("history-search-input").value.toLowerCase().trim() : "";
    const historyItems = rentalHistory.toArray();

    const filtered = historyItems.filter(h => {
        return h.bookingId.toLowerCase().includes(search) ||
               h.customerId.toLowerCase().includes(search) ||
               h.customerName.toLowerCase().includes(search) ||
               h.vehicleId.toLowerCase().includes(search);
    });

    if (filtered.length === 0) {
        tbody.innerHTML = `<tr><td colspan="6" class="text-center py-8 text-slate-500">No rental history records found.</td></tr>`;
        return;
    }

    tbody.innerHTML = filtered.map(h => `
        <tr class="hover:bg-slate-800/40 transition">
            <td class="px-4 py-3 font-mono font-bold text-indigo-400">${h.bookingId}</td>
            <td class="px-4 py-3 font-medium text-white">${h.customerName} <span class="text-slate-400 text-[11px]">(${h.customerId})</span></td>
            <td class="px-4 py-3">${h.vehicleName} <span class="text-slate-400 text-[11px]">(${h.vehicleId})</span></td>
            <td class="px-4 py-3 font-mono">${h.rentalDays} days</td>
            <td class="px-4 py-3 font-bold text-emerald-400">&#8377;${h.totalRent.toFixed(2)}</td>
            <td class="px-4 py-3">
                <span class="px-2 py-0.5 rounded-full text-[10px] font-bold ${h.status === 'Completed' ? 'bg-emerald-500/10 text-emerald-400 border border-emerald-500/20' : 'bg-amber-500/10 text-amber-400 border border-amber-500/20'}">
                    ${h.status}
                </span>
            </td>
        </tr>
    `).join("");
}

// ==========================================
// 5. BOOKING & RETURN FLOWS
// ==========================================

function openBookModal(vehicleId) {
    const v = vehicles.find(item => item.id === vehicleId);
    if (!v) return;
    if (!v.isAvailable) {
        promptJoinQueue(vehicleId);
        return;
    }

    selectedVehicleForBooking = v;
    const info = document.getElementById("book-modal-vehicle-info");
    info.innerHTML = `
        <div class="flex justify-between font-bold text-white">
            <span>${v.name} (${v.id})</span>
            <span class="text-emerald-400">&#8377;${v.rentPerDay}/day</span>
        </div>
        <p class="text-slate-400">${v.brand} &bull; ${v.type}</p>
    `;

    // Populate customers
    const select = document.getElementById("book-customer-select");
    select.innerHTML = customers.map(c => `
        <option value="${c.id}">${c.name} (${c.id})</option>
    `).join("");

    updateRentEstimate();
    openModal("bookVehicleModal");
}

function prefillBookingForCustomer(customerId) {
    switchTab("fleet");
    showToast(`Select an available vehicle for customer ${customerId}`, "info");
}

function updateRentEstimate() {
    if (!selectedVehicleForBooking) return;
    const days = parseInt(document.getElementById("book-rental-days").value) || 1;
    const v = selectedVehicleForBooking;
    const total = v.calculateRent(days);

    document.getElementById("book-modal-daily-rate").innerHTML = `&#8377;${v.rentPerDay.toFixed(2)}`;
    
    let modifierText = "Standard Rate";
    if (v.type === "Car" && days >= 7) modifierText = "5% Weekly Discount Applied";
    if (v.type === "Bike" && (v.category === "Sport" || v.category === "Cruiser")) modifierText = "+Rs 50 Helmet & Riding Gear";
    if (v.type === "Scooter" && v.isElectric) modifierText = "5% Green EV Subsidy Applied";

    document.getElementById("book-modal-modifier").innerText = modifierText;
    document.getElementById("book-modal-total-rent").innerHTML = `&#8377;${total.toFixed(2)}`;
}

function handleBookSubmit(e) {
    e.preventDefault();
    if (!selectedVehicleForBooking) return;

    const cId = document.getElementById("book-customer-select").value;
    const days = parseInt(document.getElementById("book-rental-days").value);
    const customer = customers.find(c => c.id === cId);
    const vehicle = selectedVehicleForBooking;

    if (!customer || days <= 0) return;

    if (!vehicle.isAvailable) {
        closeModal("bookVehicleModal");
        promptJoinQueue(vehicle.id);
        return;
    }

    const totalRent = vehicle.calculateRent(days);
    const bId = "BK" + (1000 + activeBookings.length + rentalHistory.recordCount + 1);

    // Update status
    vehicle.isAvailable = false;

    // Create booking
    const booking = new Booking(bId, customer.id, customer.name, vehicle.id, vehicle.name, days, totalRent, "Active");
    activeBookings.push(booking);
    rentalHistory.insertRecord(booking);

    saveState();
    closeModal("bookVehicleModal");
    refreshAllViews();

    showToast(`Booking ${bId} confirmed for ${vehicle.name}!`, "success");
}

function promptJoinQueue(vehicleId) {
    const vehicle = vehicles.find(v => v.id === vehicleId);
    if (!vehicle) return;

    if (customers.length === 0) {
        showToast("Please register a customer first.", "error");
        return;
    }

    const cName = prompt(`Vehicle ${vehicle.name} (${vehicleId}) is currently rented.\n\nEnter Customer ID to add to Waitlist (e.g. C101):`);
    if (!cName) return;

    const customer = customers.find(c => c.id.toUpperCase() === cName.trim().toUpperCase());
    if (!customer) {
        showToast("Customer ID not found in directory.", "error");
        return;
    }

    waitingQueue.enqueue(customer.id, customer.name, vehicleId);
    saveState();
    refreshAllViews();
    showToast(`Added ${customer.name} to waitlist for ${vehicle.name}!`, "info");
}

function openReturnModal(bookingId) {
    const booking = activeBookings.find(b => b.bookingId === bookingId);
    if (!booking) return;

    selectedBookingForReturn = booking;
    const info = document.getElementById("return-modal-booking-info");
    info.innerHTML = `
        <div class="flex justify-between font-bold text-white">
            <span>${booking.bookingId} - ${booking.vehicleName}</span>
            <span class="text-cyan-400">${booking.customerName}</span>
        </div>
        <p class="text-slate-400">Scheduled: ${booking.rentalDays} days &bull; Initial Est: &#8377;${booking.totalRent.toFixed(2)}</p>
    `;

    document.getElementById("return-actual-days").value = booking.rentalDays;
    updateReturnSettlement();
    openModal("returnVehicleModal");
}

function updateReturnSettlement() {
    if (!selectedBookingForReturn) return;
    const actualDays = parseInt(document.getElementById("return-actual-days").value) || selectedBookingForReturn.rentalDays;
    const scheduledDays = selectedBookingForReturn.rentalDays;
    const v = vehicles.find(item => item.id === selectedBookingForReturn.vehicleId);

    let finalAmount = selectedBookingForReturn.totalRent;
    let lateFee = 0;

    if (v) {
        finalAmount = v.calculateRent(actualDays);
        if (actualDays > scheduledDays) {
            lateFee = (actualDays - scheduledDays) * 100; // Rs 100/day penalty
            finalAmount += lateFee;
        }
    }

    document.getElementById("return-scheduled-days").innerText = `${scheduledDays} days`;
    document.getElementById("return-late-penalty").innerHTML = lateFee > 0 ? `<span class="text-rose-400">+&#8377;${lateFee.toFixed(2)}</span>` : `&#8377;0.00`;
    document.getElementById("return-final-amount").innerHTML = `&#8377;${finalAmount.toFixed(2)}`;
}

function handleReturnSubmit(e) {
    e.preventDefault();
    if (!selectedBookingForReturn) return;

    const booking = selectedBookingForReturn;
    const actualDays = parseInt(document.getElementById("return-actual-days").value) || booking.rentalDays;
    const v = vehicles.find(item => item.id === booking.vehicleId);

    let finalAmount = booking.totalRent;
    if (v) {
        finalAmount = v.calculateRent(actualDays);
        if (actualDays > booking.rentalDays) {
            finalAmount += (actualDays - booking.rentalDays) * 100;
        }
    }

    if (v) v.isAvailable = true;
    rentalHistory.updateStatus(booking.bookingId, "Completed", finalAmount);
    activeBookings = activeBookings.filter(b => b.bookingId !== booking.bookingId);

    saveState();
    closeModal("returnVehicleModal");
    refreshAllViews();

    showToast(`Vehicle returned and settlement of Rs. ${finalAmount.toFixed(2)} completed!`, "success");

    // Check Waitlist for waiting customer
    if (v && waitingQueue.hasCustomerForVehicle(v.id)) {
        setTimeout(() => {
            const nextInQueue = waitingQueue.serveCustomerForVehicle(v.id);
            if (nextInQueue) {
                const autoBook = confirm(`Waitlist Alert!\n\nCustomer ${nextInQueue.customerName} (${nextInQueue.customerId}) was waiting for ${v.name} (${v.id}).\n\nWould you like to book this vehicle for them now?`);
                if (autoBook) {
                    saveState();
                    refreshAllViews();
                    openBookModal(v.id);
                    document.getElementById("book-customer-select").value = nextInQueue.customerId;
                } else {
                    saveState();
                    refreshAllViews();
                }
            }
        }, 400);
    }
}

// ==========================================
// 6. ADD VEHICLE & CUSTOMER HANDLERS
// ==========================================

function toggleVehicleSpecificFields() {
    const type = document.getElementById("new-vehicle-type").value;
    document.getElementById("car-fields").classList.toggle("hidden", type !== "Car");
    document.getElementById("bike-fields").classList.toggle("hidden", type !== "Bike");
    document.getElementById("scooter-fields").classList.toggle("hidden", type !== "Scooter");
}

function handleAddVehicleSubmit(e) {
    e.preventDefault();
    const type = document.getElementById("new-vehicle-type").value;
    const id = document.getElementById("new-vehicle-id").value.trim().toUpperCase();
    const brand = document.getElementById("new-vehicle-brand").value.trim();
    const name = document.getElementById("new-vehicle-name").value.trim();
    const rent = parseFloat(document.getElementById("new-vehicle-rent").value);

    if (vehicles.some(v => v.id.toUpperCase() === id)) {
        showToast(`Vehicle ID ${id} already exists!`, "error");
        return;
    }

    let newV;
    if (type === "Car") {
        const seats = document.getElementById("new-car-seats").value;
        const fuel = document.getElementById("new-car-fuel").value;
        newV = new Car(id, name, brand, rent, seats, fuel, true);
    } else if (type === "Bike") {
        const cc = document.getElementById("new-bike-cc").value;
        const cat = document.getElementById("new-bike-category").value;
        newV = new Bike(id, name, brand, rent, cc, cat, true);
    } else if (type === "Scooter") {
        const boot = document.getElementById("new-scooter-boot").value;
        const isEv = document.getElementById("new-scooter-electric").value === "true";
        newV = new Scooter(id, name, brand, rent, boot, isEv, true);
    }

    vehicles.push(newV);
    saveState();
    closeModal("addVehicleModal");
    document.getElementById("addVehicleForm").reset();
    refreshAllViews();
    showToast(`Added ${name} (${id}) to fleet!`, "success");
}

function handleAddCustomerSubmit(e) {
    e.preventDefault();
    const name = document.getElementById("new-customer-name").value.trim();
    const phone = document.getElementById("new-customer-phone").value.trim();

    const newId = "C" + (100 + customers.length + 1);
    const newCust = new Customer(newId, name, phone);

    customers.push(newCust);
    saveState();
    closeModal("addCustomerModal");
    document.getElementById("addCustomerForm").reset();
    refreshAllViews();
    showToast(`Customer registered: ${name} (${newId})`, "success");
}

// ==========================================
// 7. MODALS & TOAST NOTIFICATIONS
// ==========================================

function openModal(id) {
    const el = document.getElementById(id);
    if (el) {
        el.classList.remove("hidden");
        el.classList.add("flex");
    }
}

function closeModal(id) {
    const el = document.getElementById(id);
    if (el) {
        el.classList.add("hidden");
        el.classList.remove("flex");
    }
}

function showToast(message, type = "success") {
    const toast = document.getElementById("toast");
    const msg = document.getElementById("toast-message");
    const icon = document.getElementById("toast-icon");

    msg.innerText = message;
    if (type === "success") {
        icon.className = "fa-solid fa-circle-check text-emerald-400 text-base";
    } else if (type === "error") {
        icon.className = "fa-solid fa-circle-exclamation text-rose-400 text-base";
    } else {
        icon.className = "fa-solid fa-circle-info text-cyan-400 text-base";
    }

    toast.classList.remove("translate-y-20", "opacity-0");
    setTimeout(() => {
        toast.classList.add("translate-y-20", "opacity-0");
    }, 3500);
}

// Global Init
window.addEventListener("DOMContentLoaded", () => {
    loadState();
    refreshAllViews();
});
