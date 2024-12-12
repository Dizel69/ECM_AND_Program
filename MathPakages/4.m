# Ур-е параболического типа, неявная схема, на границе заданы производные

h = 0.25;
tau = 0.01;

x_border = 1;
t_border = 0.02;

x = 0:h:x_border;
t = 0:tau:t_border;
a = 1;
n = length(x);
m = length(t);
func = 0;

function alpha = alphaBorder(t)
  alpha = 1 / (1 + t);
endfunction

function beta = betaBorder(t)
  beta = -1 / (1 + t);
endfunction

M1 = -h * alphaBorder(t(1));
M2 = h * betaBorder(t(1));
K1 = 1;
K2 = 1;

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

for j = 2:m;
  M1 = -h * alphaBorder(t(j));
  M2 = h * betaBorder(t(j));
  
  alpha(j-1,1) = K1;
  beta(j-1,1) = M1;
  
  for i = 1:n-2;
    fi(j-1,i) = temp_table(j-1,i+1) + tau*func;
  end
  
  for i = 1:n-2;
    alpha(j-1,i+1) = - (bi/(ci + ai*alpha(j-1,i)));
    beta(j-1,i+1) = (fi(j-1,i) - ai * beta(j-1,i))/(ci + ai*alpha(j-1,i));
  end
  
  temp_table(j,n) = (M2 + K2*beta(j-1,n-1))/(1 - K2*alpha(j-1,n-1));
  for i = n:-1:2;
    temp_table(j,i-1) = alpha(j-1,i-1)*temp_table(j,i) + beta(j-1,i-1);
  end
end

x_vector = [0:h:x_border];
table = [x_vector; temp_table];

t_vector = [0, 0:tau:t_border;]'
table = [t_vector table];


alpha
beta
table
fi