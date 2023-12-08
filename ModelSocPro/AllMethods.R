#additive
additiveMethod=function(open, period, index) {
  len = length(open)
  if (period > len) {
    print("                      ")
    return()
  }
  t = 1:len
  
  info = lm(open~t)
  a = info$coefficients[1]
  b = info$coefficients[2]
  
  trend = a + b * t
  S = open - trend
  
  St = rep(0, period)
  for (i in 1:period) {
    for (j in seq(i, len, by = period)) {
      St[i] = St[i] + S[j]
    }
    St[i] = St[i] / floor((((len - i) / period) + 1))
  }
  
  t = c(t, (length(t): (length(t) + period)) )
  trend = a + b * t
  
  forecast = c()
  for (k in 0:length(trend))
  {
    index = k %% (length(St)) + 1
    forecast = c(forecast, (trend[i] + St[index]))
  }
  
  error = open - forecast
  absoluteError = abs(error)
  squareError = error * error
  sumSquareError = sum(squareError)
  
  MAE = mean(absoluteError)
  APE = (absoluteError / open) * 100
  MAPE = mean(APE)
  MSE = (1/(len)) * sumSquareError
  RMSE = MSE^(1/2)
  
  return(list(MAE, MAPE, RMSE, forecast))
}
#BrownLinTrend
BrownsLinearTrend=function(open, beta) {
  len = length(open)
  
  u = open[1]
  b = open[2] - open[1]
  e = 0
  
  for(i in 2:len) {
    u[i] = u[i - 1] + b[i - 1] + (1 - beta * beta) * e[i - 1]
    b[i] = b[i - 1] + (1 - beta) * (1 - beta) * e[i - 1]
    e[i] = open[i] - u[i]
  }
  
  forecast = u
  error = e[3:len]
  
  absoluteError = abs(error)
  
  MAE = mean(absoluteError)
  APE = (absoluteError / open[3:len]) * 100
  MAPE = mean(APE)
  
  MSE = sum(error * error) / (len - 4)
  RMSE = sqrt(MSE)
  
  u = forecast[len] + b[len] + (1 - beta * beta) * e[len]
  b = b[len] + (1 - beta) * (1 - beta) * e[len]
  tail = u + b * (0:4)
  forecast = c(forecast, tail)
  
  return (list(MAE, MAPE, RMSE, forecast))
  
}
#BrownSimple
BrownsSimple=function(open, alpha) {
  len = length(open)
  forecast = open[1] 
  
  
  for (i in 1:(len - 1)) {
    forecast = c(forecast, alpha * open[i] + (1 - alpha) * forecast[i])
  }
  
  error = open - forecast
  absoluteError = abs(error)
  squaredError = error * error
  
  MAE = mean(absoluteError[2:len])
  
  APE = (absoluteError / open) * 100
  MAPE = mean(APE[2:len])
  
  MSE = sum(squaredError[2:len]) / (length(squaredError[2:len]) - 2)
  RMSE = sqrt(MSE)
  
  tail = rep(alpha * open[len] + (1 - alpha) * forecast[len], 5)
  forecast = c(forecast, tail)
  
  return (list(MAE, MAPE, RMSE, forecast))
}
#HoltMethod
HoltMethod=function(open, alpha, beta) {
  len = length(open)
  
  u = open[1]
  b = open[2] - open[1]
  
  for(i in 2:len) {
    u[i] = alpha * open[i - 1] + (1 - alpha) * (u[i - 1] + b[i - 1])
    b[i] = beta * (u[i] - u[i - 1]) + (1 - beta) * b [i - 1]
  }
  
  forecast = u
  error = (open - forecast)[2:len]
  
  absoluteError = abs(error)
  
  MAE = mean(absoluteError)
  APE = (absoluteError / open[2:len]) * 100
  MAPE = mean(APE)
  
  MSE = sum(error * error) / (len - 3)
  RMSE = sqrt(MSE)
  
  u = alpha * open[len] + (1 - alpha) * (forecast[len] + b[len])
  b = beta * (u - forecast[len]) + (1 - beta) * b[len]
  tail = u + b * (0:4)
  forecast = c(forecast, tail)
  
  return (list(MAE, MAPE, RMSE, forecast))
}
#LinnearTrend
linearTrend=function(open) {
  len = length(open)
  t = 1:len
  
  trend = lm(open ~ t)
  a = trend$coefficients[1]
  b = trend$coefficients[2]
  
  forecast = a + b * t
  
  error = open - forecast
  absoluteError = abs(error)
  
  MAE = mean(absoluteError)
  APE = (absoluteError / open) * 100
  MAPE = mean(APE)
  
  MSE = sum(error * error) / (len - 2)
  RMSE = sqrt(MSE)
  
  tail = a + b * ((len + 1):(len + 5))
  forecast = c(forecast, tail)
  
  return (list(MAE, MAPE, RMSE, forecast))
}
#MeanMethod
meanMethod=function(open) {
  len = length(open)
  forecast = rep(mean(open), len)
  
  error = open - forecast
  absoluteError = abs(error)
  squaredError = error * error
  
  MAE = mean(absoluteError)
  
  APE = (absoluteError / open) * 100
  MAPE = mean(APE)
  
  MSE = sum(squaredError) / (length(squaredError) - 2)
  RMSE = sqrt(MSE)
  
  forecast = c(forecast, rep(mean(open), 5))
  
  return (list(MAE, MAPE, RMSE, forecast))
}
#MovingAverage
movingAverage=function(open, p) {
  len = length(open)
  forecast = rep(NA, p)
  
  for (i in (p+1):len) {
    forecast = c(forecast, mean( open[(i - p):(i - 1)] ))
  }
  
  error = open - forecast
  absoluteError = abs(error)
  squaredError = error * error
  
  MAE = mean(absoluteError[(p + 1):len])
  
  APE = (absoluteError / open) * 100
  MAPE = mean(APE[(p + 1):len])
  
  MSE = sum(squaredError[(p + 1):len]) / (length(squaredError[(p + 1):len]) - 2)
  RMSE = sqrt(MSE)
  
  tail = rep(mean(open[(len - p + 1):len]), 5)
  forecast = c(forecast, tail)
  
  return (list(MAE, MAPE, RMSE, forecast))
}
#MultiplicativeMethod
multiplicativeMethod=function(open, period, index) {
  if (period > length(open)) {
    print("Период больше длины")
    return()
  }
  len = length(open)
  
  t = 1:len
  
  info = lm(open~t)
  a = info$coefficients[1]
  b = info$coefficients[2]
  
  trend = a + b * t
  S = open / trend
  
  St = rep(0, period)
  for (i in 1:period) {
    for (j in seq(i, len, by = period)) {
      St[i] = St[i] + S[j]
    }
    St[i] = St[i] / floor((((len - i) / period) + 1))
  }
  St = St / (prod(St)^(1/length(St)))
  
  forecast = c()
  for (k in 0:(length(trend) - 1))
  {
    ind = k %% (length(St)) + 1
    
    forecast = c(forecast, (trend[k + 1] * St[ind]))
  }
  
  error = open - forecast
  absoluteError = abs(error)
  squareError = error * error
  sumSquareError = sum(squareError)
  
  MAE = mean(absoluteError)
  APE = (absoluteError / open) * 100
  MAPE = mean(APE)
  MSE = (1/(len)) * sumSquareError
  RMSE = MSE^(1/2)
  
  t = c(t, ((length(t) + 1): (length(t) + period)) )
  trend = a + b * t
  
  forecast = c()
  for (k in 0:(length(trend) - 1))
  {
    ind = k %% (length(St)) + 1
    
    forecast = c(forecast, (trend[k + 1] * St[ind]))
  }
  results = c(MAE, MAPE, RMSE)
  #return(list(results[index], forecast, "multiplicativeMethod", period))
  return (list(MAE, MAPE, RMSE, forecast))
}
#PrevMeanMethod
prevMeanMethod=function(open) {
  len = length(open)
  forecast = c(NA)
  
  for (i in 1:(len - 1)) {
    forecast = c(forecast, mean(open[1:i]))
  }
  
  error = open - forecast
  absoluteError = abs(error)
  squaredError = error * error
  
  MAE = mean(absoluteError[2:len])
  
  APE = (absoluteError / open) * 100
  MAPE = mean(APE[2:len])
  
  MSE = sum(squaredError[2:len]) / (length(squaredError[2:len]) - 2)
  RMSE = sqrt(MSE)
  
  forecast = c(forecast, rep(mean(open), 5))
  
  return (list(MAE, MAPE, RMSE, forecast))
}
#RandomWalk
randomWalk=function(open) {
  len = length(open)
  forecast = c(NA, open[1:len - 1])
  
  error = open - forecast
  absoluteError = abs(error)
  squaredError = error * error
  
  MAE = mean(absoluteError[2:len])
  
  APE = (absoluteError / open) * 100
  MAPE = mean(APE[2:len])
  
  MSE = sum(squaredError[2:len]) / (length(squaredError[2:len]) - 2)
  RMSE = sqrt(MSE)
  
  tail = rep(open[len], 5)
  forecast = c(forecast, tail)
  
  return (list(MAE, MAPE, RMSE, forecast))
}
#RandomWalkWithDrift
randomWalkWithDrift=function(open) {
  len = length(open)
  forecast = c(NA, open[1:len - 1])
  
  drift = (open[len] - open[1]) / (len - 1)
  
  forecast = forecast + drift
  
  error = open - forecast
  absoluteError = abs(error)
  squaredError = error * error
  
  MAE = mean(absoluteError[2:len])
  
  APE = (absoluteError / open) * 100
  MAPE = mean(APE[2:len])
  
  MSE = sum(squaredError[2:len]) / (length(squaredError[2:len]) - 2)
  RMSE = sqrt(MSE)
  
  tail = open[len] + (1:5) * drift
  forecast = c(forecast, tail)
  
  return (list(MAE, MAPE, RMSE, forecast))
}

#Тейл-Вейдж
TheilWage = function(open, period, index) {
  if (period > length(open)) {
    print("The period is too long")
    return()
  }
  len = length(open)
  
  t = 1:len
  
  info = lm(open~t)
  a = as.numeric(info$coefficients[1])
  b = as.numeric(info$coefficients[2])
  
  trend = a + b * t
  
  S = open - trend
  
  St = rep(0, period)
  for (i in 1:period) {
    for (j in seq(i, len, by = period)) {
      St[i] = St[i] + S[j]
    }
    St[i] = St[i] / floor((((len - i) / period) + 1))
  }
  St = St - mean(St)
  
  bestAlpha = 0.01
  bestBeta = 0.01
  bestGamma = 0.01
  bestParameter = 100
  for (alpha in seq(0.01, 0.99, by = 0.01)) {
    for (beta in seq(0.01, 0.99, by = 0.01)) {
      for (gamma in seq(0.01, 0.99, by = 0.01)) {
        S_T = St
        at = a
        bt = b
        
        
        alpha = 0.999
        beta = 0.04316
        gamma = 0.001
        #bestParameter = 100
        for (i in 2:(len + 1)) {
          at = c(at, alpha * (open[i - 1] - S_T[i - 1]) + (1 - alpha) * (at[i  - 1] + bt[i - 1]) + bt[i - 1])
          bt = c(bt, beta * (at[i] - at[i - 1]) + (1 - beta)  * bt[i - 1])
          S_T = c(S_T, gamma * (open[i - 1] - at[i]) + (1 - gamma) * S_T[i - 1])
        }
        
        forecast = c()
        for(i in 1:len) {
          forecast = c(forecast, (at[i + 1] + bt[i + 1]) + S_T[i])
        }
        
        error = open - forecast
        absoluteError = abs(error)
        squareError = error * error
        sumSquareError = sum(squareError)
        
        MAE = mean(absoluteError)
        APE = (absoluteError / open) * 100
        MAPE = mean(APE)
        MSE = (1/(len)) * sumSquareError
        RMSE = MSE^(1/2)
        
        results = c(MAE, MAPE, RMSE)
        
        if (results[index] < bestParameter) {
          bestParameter = results[index]
          bestAlpha = alpha
          bestBeta = beta
          bestGamma = gamma
          #cat("cur:", results[index], "; best:", bestParameter, "alpha:", alpha, "beta:", beta, "gamma:", gamma, "\n")
          }
        
      }
    }
  }
  
  t = c(t, 1:period)
  
  for (i in 1:period) {
    #cat(at[length(at)], bt[length(bt)], t[len + i], S_T[len + i], "\n")
    forecast = c(forecast, (at[length(at)] + bt[length(bt)] * t[len + i]) + S_T[len + i])
  }
  
  cat(bestAlpha, bestBeta, bestGamma)
  #return(list(results[index], forecast, "TheilWage", period))
  return (list(MAE, MAPE, RMSE, forecast))
  }

#Холт-Винтер
HoltWinter = function(open, period, index){
  if (period > length(open)) {
    print("The period is too long")
    return()
  }
  len = length(open)
  
  t = 1:len
  
  info = lm(open~t)
  a = as.numeric(info$coefficients[1])
  b = as.numeric(info$coefficients[2])
  
  trend = a + b * t
  
  S = open / trend
  
  St = rep(0, period)
  for (i in 1:period) {
    for (j in seq(i, len, by = period)) {
      St[i] = St[i] + S[j]
    }
    St[i] = St[i] / floor((((len - i) / period) + 1))
  }
  St = St / (prod(St)^(1/length(St)))
  
  bestAlpha = 0.01
  bestBeta = 0.01
  bestGamma = 0.01
  bestParameter = 100
  for (alpha in seq(0.01, 0.99, by = 0.01)) {
    for (beta in seq(0.01, 0.99, by = 0.01)) {
      for (gamma in seq(0.01, 0.99, by = 0.01)) {
        S_T = St
        at = a
        bt = b
        for (i in 2:(len + 1)) {
          at = c(at, alpha * (open[i - 1] / S_T[i - 1]) + (1 - alpha) * (at[i - 1] + bt[i - 1]) + bt[i - 1])
          bt = c(bt, beta * (at[i] - at[i - 1]) + (1 - beta)  * bt[i - 1])
          S_T = c(S_T, gamma * (open[i - 1] / at[i]) + (1 - gamma) * S_T[i - 1])
        }
        
        forecast = c()
        for(i in 1:len) {
          forecast = c(forecast, (at[i + 1] + bt[i + 1]) * S_T[i])
        }
        
        error = open - forecast
        absoluteError = abs(error)
        squareError = error * error
        sumSquareError = sum(squareError)
        
        MAE = mean(absoluteError)
        APE = (absoluteError / open) * 100
        MAPE = mean(APE)
        MSE = (1/(len)) * sumSquareError
        RMSE = MSE^(1/2)
        
        #results = c(MAE, MAPE, RMSE)
        
        
      }
    }
  }


 t = c(t, 1:4)

 for (i in 1:4) {
   forecast = c(forecast, (at[length(at)] + bt[length(bt)] * t[len + i]) * S_T[len + i])
 }
 results = c(MAE, MAPE, RMSE)

 #cat(bestAlpha, bestBeta, bestGamma)
 #return(list(results[index], forecast, "HoltWinter", period))
 return (list(MAE, MAPE, RMSE, forecast))
}
