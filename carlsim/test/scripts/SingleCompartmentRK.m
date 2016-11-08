s.C = 100.0; 
s.k = 3;
s.vr = -60.0;
s.vt = -50.0;
s.vpeak = 50;
s.a = 0.01;
s.b = 5;
s.c = -60.0;
s.d = 400.0;

T = 1000; %Total length of simulation in ms
tau = .05; %Timestep is 1 ms
n = round(T/tau); %Number of timesteps
zeros1 = zeros(1, 100/tau);
one = 300 * ones(1, 500/tau);
zeros2 = zeros(1, 400/tau);

zeros3 = zeros(1, 600/tau);

s.Ie = cat(2, cat(2, zeros1, one), zeros2); %External current

%Initialize state variables for somatic compartment
s.v = [s.vr * ones(1, n)];
s.u = 0 * s.v;
%s.Id = [zeros(1, n)]; %dendritic current
s.Is = [zeros(1, n)]; %synaptic currrent
s.I = s.Is + s.Ie;

for i = 1:n-1
    s.k1 = tau * (s.k * (s.v(i) - s.vr) * (s.v(i) - s.vt) - s.u(i) + s.I(i))/s.C;
    s.l1 = tau * s.a * (s.b*(s.v(i) - s.vr) - s.u(i));
    s.k2 = tau * (s.k * ((s.v(i) + s.k1/2) - s.vr) * ((s.v(i) + s.k1/2) - s.vt) - (s.u(i) + s.l1/2) + s.I(i))/s.C;
    s.l2 = tau * s.a * (s.b*((s.v(i) + s.k1/2) - s.vr) - (s.u(i) + s.l1/2));
    s.k3 = tau * (s.k * ((s.v(i) + s.k2/2) - s.vr) * ((s.v(i) + s.k2/2) - s.vt) - (s.u(i) + s.l2/2) + s.I(i))/s.C;
    s.l3 = tau * s.a * (s.b*((s.v(i) + s.k2/2) - s.vr) - (s.u(i) + s.l2/2));
    s.k4 = tau * (s.k * ((s.v(i) + s.k3) - s.vr) * ((s.v(i) + s.k3) - s.vt) - (s.u(i) + s.l3) + s.I(i))/s.C;
    s.l4 = tau * s.a * (s.b*((s.v(i) + s.k3) - s.vr) - (s.u(i) + s.l3));
    s.v(i + 1) = s.v(i) + (1/6) * (s.k1 + 2 * s.k2 + 2 * s.k3 + s.k4);
    s.u(i + 1) = s.u(i) + (1/6) * (s.l1 + 2 * s.l2 + 2 * s.l3 + s.l4);

    s.I(i + 1) = s.Is(i) + s.Ie(i);
    
    if s.v(i + 1) >= s.vpeak
        s.v(i) = s.vpeak;
        s.v(i + 1) = s.c;
        s.u(i + 1) = s.u(i + 1) + s.d;
    end;  
end;
figure(1);
plot(tau*(1:n), s.v);
title('Soma Voltage');
axis([0 T -70 20])

