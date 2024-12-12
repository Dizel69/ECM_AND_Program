# Ур-е гиперболического типа, на границе заданы функции

h = 0.125;
tau = 0.05;

x_border = 1;
t_border = 0.3;

x = 0:h:x_border;
t = 0:tau:t_border;
a = 1;
n = length(x);
m = length(t);
f = 0;

u = zeros(m,n);

function leftBorder = LB(t)
  leftBorder = 0;
endfunction

function rightBorder = RB(t)
  rightBorder = 1 - t^2;
endfunction

for i = 1:n;
  u(1,i) = x(i)^2;
end

u(2,1) = LB(t);
 for i = 2:n-1;
   u(2,i) = u(2-1,i) + tau * sin(pi * x(i));
 end
 u(2,n) = RB(t(2));

for j = 3:m;
  u(j,1) = 0;
  for i = 2:n-1;
    u(j,i) = 2 * u(j-1,i) - u(j-2,i) + ( (tau^2 * a^2) / (h^2) ) * (u(j-1, i-1) - 2 * u(j-1,i) + u(j-1, i+1)) + tau^2 * f;
  end
  u(j,n) = RB(t(j));
end


x_vector = [0:h:x_border];
table = [x_vector; u];

t_vector = [0, 0:tau:t_border;]';
table = [t_vector table];

table