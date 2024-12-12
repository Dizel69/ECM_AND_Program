# Ур-е параболического типа, неявная схема, на границе заданы функции

h = 0.125;
tau = 0.005;
x = 0:h:1;
t= 0:tau:0.02;
a = 1;
n = length(x);
m = length(t);
func = 0;

M1 = 0;
M2 = 0;
K1 = 0;
K2 = 0;

temp_table = zeros(m, n);

fi = zeros(m-1, n-2);
alpha = zeros(m-1, n-1);
beta = zeros(m-1, n-1);

for i = 1:n;
  temp_table(1,i) = sin(pi * x(i));
end


ai = -(tau*(a^2))/(h^2);
bi = ai;
ci = 1 + (2*tau*(a^2))/(h^2);

alpha(1,1) = K1;
beta(1,1) = M1;

for j = 2:m;
  for i = 1:n-2;
    fi(j-1,i) = temp_table(j-1,i+1) + tau*func;
  end
  
  for i = 1:n-2;
    alpha(j-1,i+1) = - (bi/(ci + ai*alpha(j-1,i)))
    beta(j-1,i+1) = (fi(j-1,i) - ai * beta(j-1,i))/(ci + ai*alpha(j-1,i)) 
  end
  
  temp_table(j,n) = (M2 + K2*beta(j-1,n-1))/(1 - K2*alpha(j-1,n-1));
  for i = n:-1:2;
    temp_table(j,i-1) = alpha(j-1,i-1)*temp_table(j,i) + beta(j-1,i-1);
  end
end

temp_table
fi