x_t = c(1, 4, 7, 9, 7, 3, 8, 10, 12, 13, 14, 15, 9, 10, 11)

model = arima(x_t, order=c(3, 1, 3))

print(model)

coef = model$coef

print("-----------------------------")
print (coef)

se = sqrt(diag(model$var.coef))

print("---------------------------------------===========================")
print(se)

t = abs(coef/se)

print(t)

p_value = 2 * (1 - pt(t, length(x_t) - length(coef)))

print("++++++++++++++++++++++++++++++++++++++")
print(p_value)

res