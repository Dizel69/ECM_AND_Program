h = 0.11
tau = 0.01
t= 0:tau:0.1
a = 1
n = floor(1/h + 1)
m = length(t)

temp_table = zeros(m, n)
x = 0:h:1

for i = 2:n-1
  temp_table(1,i) = sin(pi * x(i))
end

for j = 1:m-1
  for i = 2:n-1
    temp_table(j+1, i) = temp_table(j, i) + (((tau * a^2) / h^2) * (temp_table(j, i-1) - 2 * temp_table(j, i) + temp_table(j, i+1)))
  end
   temp_table(j+1, 1) = temp_table(j+1, 2) - h * (1 / (t(j) + 1))
   temp_table(j+1, n) = temp_table(j+1, n-1) + h * (-(1 / (t(j) + 1)))
end

