%Reg RS Neuron
%{
C = 100.0;
vr = -60.0;
vt = -40.0;
k = 0.7;
a = 0.03;
b = -2;
c.soma = -50.0;
d = 100.0;
vpeak.soma = 35;
%}

%RS Neuron - pb23
%{
C = 100.0;
vr = -60.0;
vt = -50.0;
k = 3;
a = 0.01;
b = 5;
c.soma = -60.0;
c.dendr = -55.0;
d = 400.0;
vpeak.soma = 50;
vpeak.dendr = 30;
GU = 3.0;
GD = 5.0;
%}

%FS Neuron - b
%{
C = 20.0;
k = 1;
vr = -55.0;
vt = -40.0;
vpeak.soma = 25;
vpeak.dendr = 25;
GU = 0.5;
GD = 1.0;
a = 0.15;
b = 8;
c.soma = -55.0;
c.dendr = -55.0;
d = 200.0;
%}

%LTS Neuron - nb
%{
C = 100.0;
k = 1;
vr = -56.0;
vt = -42.0;
vpeak.soma = 40.0;
vpeak.dendr = 40.0;
GU = 1.0;
GD = 1.0;
a = 0.03;
b = 8;
c.soma = -50.0;
c.dendr = -50.0;
d = 20.0;
%}

%TC Tonic Neuron
%{
C = 200.0;
k = 1.6;
vr = -60.0;
vt = -50.0;
vpeak.soma = 40.0;
vpeak.dendr = 40.0;
GU = 2.0;
GD = 2.0;
a = 0.1;
b = 15;
c.soma = -60.0;
c.dendr = -60.0;
d = 10.0;
%}

%GMU Data 3 compartments
%{
s.C = 551.0;
s.k = 3.9752;
s.vr = -59.66;
s.vt = -53.16855;
s.vpeak = 34.926968;
s.GU = 5.2;
s.GD = 12.9;
s.a = 0.0014259194;
s.b = -11.811985;
s.c = -54.317677;
s.d = 106.0;

d1.C = 555.0;
d1.k = 1.2633;
d1.vr = -59.66081;
d1.vt = -22.832753;
d1.vpeak = 27.6363889;
d1.GU = 0.0;
d1.GD = 20.8;
d1.a = 1.0446243;
d1.b = 11.087972;
d1.c = -29.978874;
d1.d = 62.0;

d3.C = 353.0;
d3.k = 1.2054616;
d3.vr = -59.66081;
d3.vt = -17.870737;
d3.vpeak = 26.31883;
d3.GU = 30.1;
d3.GD = 0.0;
d3.a = 0.45259273;
d3.b = 3.1462605;
d3.c = -34.255947;
d3.d = 32.0;
%}

%GMU data 4 compartments

s.C = 550.0; % s == SP
s.k = 2.3330991;
s.vr = -59.101414;
s.vt = -50.428886;
s.vpeak = 24.98698;
s.GU = 28.396; 
s.GD = 49.14; 
s.a = 0.0021014998;
s.b = -0.41361538;
s.c = -53.223213;
s.d = 109.0;

d1.C = 367.0; % d1 == SR
d1.k = 1.1705916;
d1.vr = -59.101414;
d1.vt = -44.298294;
d1.vpeak = 20.274296;
d1.GU = 50.474;
d1.GD = 4.6; 
d1.a = 0.2477681;
d1.b = 3.3198094;
d1.c = -46.076824;
d1.d = 24.0;

d2.C = 425.0; % d2 == SLM
d2.k = 2.2577047;
d2.vr = -59.101414;
d2.vt = -25.137894;
d2.vpeak = 13.203414;
d2.GU = 0.0; 
d2.GD = 5.526; 
d2.a = 0.32122386;
d2.b = 0.14995363;
d2.c = -38.54892;
d2.d = 69.0;

d3.C = 225.0; %d3 == SO
d3.k = 1.109572;
d3.vr = -59.101414;
d3.vt = -36.55802;
d3.vpeak = 21.473854;
d3.GU = 116.861;
d3.GD = 0.0;
d3.a = 0.29814243;
d3.b = -4.385603;
d3.c = -40.343994;
d3.d = 21.0;

T = 1000; %Total length of simulation in ms
tau = .01; %Timestep is 20 ms
n = round(T/tau); %Number of timesteps
zeros1 = zeros(1, 100/tau);
one = 1600 * ones(1, 500/tau);
zeros2 = zeros(1, 400/tau);

zeros3 = zeros(1, 600/tau);
d2.Ie = cat(2, zeros3, zeros2); %External current

d3.Ie = cat(2, cat(2, zeros1, one), zeros2); %External current

s.Ie = cat(2, zeros3, zeros2); %External current

d1.Ie = cat(2, zeros3, zeros2); %External current

%Info for daendritic compartment D3
%d3.w = 5; %synaptic weight
%d3.g.AMPA = zeros(1, n);
%d3.g.NMDA = zeros(1, n);
%d3.temp = [(d3.v + 80)/60.0] .* [(d3.v + 80)/60.0];
%d3.Is = -[d3.g.AMPA .* (d3.v - 0) + (d3.g.NMDA .* (d3.temp) .* (d3.v - 0)) ./ (1 + d3.temp)];
d3.v = [d3.vr * ones(1, n)];
d3.u = 0 * d3.v;
d3.Id = zeros(1, n);
d3.Is = zeros(1, n);
d3.I = -d3.Id + d3.Is + d3.Ie;

%Info for daendritic compartment D2 (has an excitory synapse)
%Assume that neighboring neuron spikes every 50ms starting at time t =
%50ms.
%d2.w = 10; %synaptic weight
%d2.g.AMPA = zeros(1, n);
%d2.g.NMDA = zeros(1, n);
%d2.temp = [(d2.v + 80)/60.0] .* [(d2.v + 80)/60.0];
%d2.Is = -[d2.g.AMPA .* (d2.v - 0) + (d2.g.NMDA .* (d2.temp) .* (d2.v - 0)) ./ (1 + d2.temp)];
d2.v = [d2.vr * ones(1, n)];
d2.u = 0 * d2.v;
d2.Id = zeros(1, n);
d2.Is = zeros(1, n);
d2.I = -d2.Id + d2.Is + d2.Ie;

%Info for daendritic compartment D1
d1.v = [d1.vr * ones(1, n)];
d1.u = 0 * d1.v;
d1.Id = zeros(1, n);
d1.Is = zeros(1, n);
d1.I = -d1.Id + d1.Is + d1.Ie;

%Initialize state variables for somatic compartment
s.v = [s.vr * ones(1, n)];
s.u = 0 * s.v;
%s.Id = [zeros(1, n)]; %dendritic current
s.Id = s.GD * (s.v - d3.v);
s.Is = [zeros(1, n)]; %synaptic currrent
s.I = -s.Id + s.Is + s.Ie;

for i = 1:n-1
    s.Id(i + 1) = 0 + s.GU * (s.v(i) - d1.v(i)) + s.GD * (s.v(i) - d3.v(i));
    d3.Id(i + 1) = d3.GU * (d3.v(i) - s.v(i));
    d2.Id(i + 1) = d2.GD * (d2.v(i) - d1.v(i));
    d1.Id(i + 1) = d1.GD * (d1.v(i) - s.v(i)) + d1.GU * (d1.v(i) - d2.v(i));
    s.I(i + 1) = -s.Id(i) + s.Is(i) + s.Ie(i);
    d1.I(i + 1) = -d1.Id(i) + d1.Is(i) + d1.Ie(i);
    d2.I(i + 1) = -d2.Id(i) + d2.Is(i) + d2.Ie(i);
    d3.I(i + 1) = -d3.Id(i) + d3.Is(i) + d3.Ie(i);
    
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
    
    d3.k1 = tau * (d3.k * (d3.v(i) - d3.vr) * (d3.v(i) - d3.vt) - d3.u(i) + d3.I(i))/d3.C;
    d3.l1 = tau * d3.a * (d3.b*(d3.v(i) - d3.vr) - d3.u(i));
    d3.k2 = tau * (d3.k * ((d3.v(i) + d3.k1/2) - d3.vr) * ((d3.v(i) + d3.k1/2) - d3.vt) - (d3.u(i) + d3.l1/2) + d3.I(i))/d3.C;
    d3.l2 = tau * d3.a * (d3.b*((d3.v(i) + d3.k1/2) - d3.vr) - (d3.u(i) + d3.l1/2));
    d3.k3 = tau * (d3.k * ((d3.v(i) + d3.k2/2) - d3.vr) * ((d3.v(i) + d3.k2/2) - d3.vt) - (d3.u(i) + d3.l2/2) + d3.I(i))/d3.C;
    d3.l3 = tau * d3.a * (d3.b*((d3.v(i) + d3.k2/2) - d3.vr) - (d3.u(i) + d3.l2/2));
    d3.k4 = tau * (d3.k * ((d3.v(i) + d3.k3) - d3.vr) * ((d3.v(i) + d3.k3) - d3.vt) - (d3.u(i) + d3.l3) + d3.I(i))/d3.C;
    d3.l4 = tau * d3.a * (d3.b*((d3.v(i) + d3.k3) - d3.vr) - (d3.u(i) + d3.l3));
    d3.v(i + 1) = d3.v(i) + (1/6) * (d3.k1 + 2 * d3.k2 + 2 * d3.k3 + d3.k4);
    d3.u(i + 1) = d3.u(i) + (1/6) * (d3.l1 + 2 * d3.l2 + 2 * d3.l3 + d3.l4);
    
    d2.k1 = tau * (d2.k * (d2.v(i) - d2.vr) * (d2.v(i) - d2.vt) - d2.u(i) + d2.I(i))/d2.C;
    d2.l1 = tau * d2.a * (d2.b*(d2.v(i) - d2.vr) - d2.u(i));
    d2.k2 = tau * (d2.k * ((d2.v(i) + d2.k1/2) - d2.vr) * ((d2.v(i) + d2.k1/2) - d2.vt) - (d2.u(i) + d2.l1/2) + d2.I(i))/d2.C;
    d2.l2 = tau * d2.a * (d2.b*((d2.v(i) + d2.k1/2) - d2.vr) - (d2.u(i) + d2.l1/2));
    d2.k3 = tau * (d2.k * ((d2.v(i) + d2.k2/2) - d2.vr) * ((d2.v(i) + d2.k2/2) - d2.vt) - (d2.u(i) + d2.l2/2) + d2.I(i))/d2.C;
    d2.l3 = tau * d2.a * (d2.b*((d2.v(i) + d2.k2/2) - d2.vr) - (d2.u(i) + d2.l2/2));
    d2.k4 = tau * (d2.k * ((d2.v(i) + d2.k3) - d2.vr) * ((d2.v(i) + d2.k3) - d2.vt) - (d2.u(i) + d2.l3) + d2.I(i))/d2.C;
    d2.l4 = tau * d2.a * (d2.b*((d2.v(i) + d2.k3) - d2.vr) - (d2.u(i) + d2.l3));
    d2.v(i + 1) = d2.v(i) + (1/6) * (d2.k1 + 2 * d2.k2 + 2 * d2.k3 + d2.k4);
    d2.u(i + 1) = d2.u(i) + (1/6) * (d2.l1 + 2 * d2.l2 + 2 * d2.l3 + d2.l4);
    
    d1.k1 = tau * (d1.k * (d1.v(i) - d1.vr) * (d1.v(i) - d1.vt) - d1.u(i) + d1.I(i))/d1.C;
    d1.l1 = tau * d1.a * (d1.b*(d1.v(i) - d1.vr) - d1.u(i));
    d1.k2 = tau * (d1.k * ((d1.v(i) + d1.k1/2) - d1.vr) * ((d1.v(i) + d1.k1/2) - d1.vt) - (d1.u(i) + d1.l1/2) + d1.I(i))/d1.C;
    d1.l2 = tau * d1.a * (d1.b*((d1.v(i) + d1.k1/2) - d1.vr) - (d1.u(i) + d1.l1/2));
    d1.k3 = tau * (d1.k * ((d1.v(i) + d1.k2/2) - d1.vr) * ((d1.v(i) + d1.k2/2) - d1.vt) - (d1.u(i) + d1.l2/2) + d1.I(i))/d1.C;
    d1.l3 = tau * d1.a * (d1.b*((d1.v(i) + d1.k2/2) - d1.vr) - (d1.u(i) + d1.l2/2));
    d1.k4 = tau * (d1.k * ((d1.v(i) + d1.k3) - d1.vr) * ((d1.v(i) + d1.k3) - d1.vt) - (d1.u(i) + d1.l3) + d1.I(i))/d1.C;
    d1.l4 = tau * d1.a * (d1.b*((d1.v(i) + d1.k3) - d1.vr) - (d1.u(i) + d1.l3));
    d1.v(i + 1) = d1.v(i) + (1/6) * (d1.k1 + 2 * d1.k2 + 2 * d1.k3 + d1.k4);
    d1.u(i + 1) = d1.u(i) + (1/6) * (d1.l1 + 2 * d1.l2 + 2 * d1.l3 + d1.l4);
    
    %if(mod(i, (50/tau)) == 0) %Spike occured.
    %    d2.g.AMPA(i + 1) = d2.g.AMPA(i) + d2.w;
    %    d2.g.NMDA(i + 1) = d2.g.NMDA(i) + d2.w;
        %disp('Neighboring neuron spiked');
    %else
    %    d2.g.AMPA(i + 1) = max(0, d2.g.AMPA(i) - (d2.g.AMPA(i)/5)*tau);
    %    d2.g.NMDA(i + 1) = max(0, d2.g.NMDA(i) - (d2.g.NMDA(i)/150)*tau);
    %end
    %d2.temp(i + 1) = [(d2.v(i) + 80.0)/60.0] * [(d2.v(i) + 80.0)/60.0];
    %d2.Is(i + 1) = -[d2.g.AMPA(i) * (d2.v(i) - 0) + (d2.g.NMDA(i) * (d2.temp(i)) * (d2.v(i) - 0)) / (1 + d2.temp(i))];
    
    %if(mod(i, (70/tau)) == 0) %Spike occured.
    %    d3.g.AMPA(i + 1) = d3.g.AMPA(i) + d3.w;
    %    d3.g.NMDA(i + 1) = d3.g.NMDA(i) + d3.w;
        %disp('Neighboring neuron spiked');
    %else
    %    d3.g.AMPA(i + 1) = max(0, d3.g.AMPA(i) - (d3.g.AMPA(i)/5)*tau);
    %    d3.g.NMDA(i + 1) = max(0, d3.g.NMDA(i) - (d3.g.NMDA(i)/150)*tau);
    %end
    %d3.temp(i + 1) = [(d3.v(i) + 80.0)/60.0] * [(d3.v(i) + 80.0)/60.0];
    %d3.Is(i + 1) = -[d3.g.AMPA(i) * (d3.v(i) - 0) + (d3.g.NMDA(i) * (d3.temp(i)) * (d3.v(i) - 0)) / (1 + d3.temp(i))];
    
    if s.v(i + 1) >= s.vpeak
        s.v(i) = s.vpeak;
        s.v(i + 1) = s.c;
        s.u(i + 1) = s.u(i + 1) + s.d;
    end;
    if d1.v(i + 1) >= d1.vpeak
        d1.v(i) = d1.vpeak;
        d1.v(i + 1) = d1.c;
        d1.u(i + 1) = d1.u(i + 1) + d1.d;
    end;
    
    if d2.v(i + 1) >= d2.vpeak
        d2.v(i) = d2.vpeak;
        d2.v(i + 1) = d2.c;
        d2.u(i + 1) = d2.u(i + 1) + d2.d;
    end;
    if d3.v(i + 1) >= d3.vpeak
        d3.v(i) = d3.vpeak;
        d3.v(i + 1) = d3.c;
        d3.u(i + 1) = d3.u(i + 1) + d3.d;
    end;
    
end;
figure(1);
plot(tau*(1:n), s.v);
title('Soma Voltage');
axis([0 T -70 20])

figure(2);
plot(tau*(1:n), d3.v);
title('D3 Voltage');
axis([0 T -70 20])

figure(3);
plot(tau*(1:n), d1.v);
title('D1 Voltage');
axis([0 T -70 20])

figure(4);
plot(tau*(1:n), d2.v);
title('D2 Voltage');
axis([0 T -70 20])

%figure(5);
%plot(tau*(1:n), s.Ie);
%title('Soma External Current');
%axis([0 T 0 2000])

%figure(6);
%plot(tau*(1:n), s.Id);
%title('Soma Dendr Current');
%axis([0 T -70 20])

%figure(7);
%plot(tau*(1:n), d3.Id);
%title('D3 Dendr Current');
%axis([0 T -70 20])

%figure(8);
%plot(tau*(1:n), d1.Id);
%title('D1 Dendr Current');
%axis([0 T -70 20])

%figure(9);
%plot(tau*(1:n), d2.Id);
%title('D2 Dendr Current');
%axis([0 T -70 20])