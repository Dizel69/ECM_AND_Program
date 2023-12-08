cat("\014")
Sys.setenv(LANG = "ru")
options(warn=-1)

open = read.csv("DATA4.csv")
open = open[!is.na(open)]

period = readline(prompt="Задайте период: ")
period = strtoi(period)
pname = readline(prompt="Выберите параметр: ")
if (pname == 'MAE'){
  p = 1
} else if (pname == 'MAPE') {
  p = 2
} else if (pname == 'RMSE') {
  p = 3
} else {
  print('Неизвестный параметр. По стандарту MAPE.')
  pname = 'MAPE'
  p = 2
}
options(digits = 3)
t1 = Sys.time()
options(digits = 3)
methodsResults = list()

BrownMinMape = 100
for (alpha in seq(0.01, 0.99, by=0.0001)) {
  BrownMape = BrownsSimple(open, alpha)[[p]]
  if (BrownMape < BrownMinMape) {
    BrownMinMape = BrownMape
    bestAlpha = alpha
  }
}
alpha = bestAlpha

BrownMinMape = 100
for (beta in seq(0.01, 0.99, by=0.0001)) {
  BrownMape = BrownsLinearTrend(open, beta)[[p]]
  if (BrownMape < BrownMinMape) {
    BrownMinMape = BrownMape
    bestBeta = beta
  }
  bestBeta = beta
}
beta = bestBeta

HoltMinMape = 100
for (alpha in seq(0.01, 0.99, by=0.01)) {
  for (beta in seq(0.01, 0.99, by=0.01)) {
    HoltMape = HoltMethod(open, alpha, beta)[[p]]
    if (HoltMape < HoltMinMape) {
      HoltMinMape = HoltMape
      bestAlpha = alpha
      bestBeta = beta
    }
  }
}
alpha = bestAlpha
beta = bestBeta

methodsResults <- append(methodsResults, list(randomWalk(open)))
methodsResults <- append(methodsResults, list(randomWalkWithDrift(open)))
methodsResults <- append(methodsResults, list(meanMethod(open)))
methodsResults <- append(methodsResults, list(prevMeanMethod(open)))
methodsResults <- append(methodsResults, list(movingAverage(open, 2)))
methodsResults <- append(methodsResults, list(movingAverage(open, 3)))
methodsResults <- append(methodsResults, list(movingAverage(open, 4)))
methodsResults <- append(methodsResults, list(movingAverage(open, 5)))
methodsResults <- append(methodsResults, list(BrownsLinearTrend(open, beta)))
methodsResults <- append(methodsResults, list(BrownsSimple(open, alpha)))
methodsResults <- append(methodsResults, list(linearTrend(open)))
methodsResults <- append(methodsResults, list(HoltMethod(open, alpha, beta)))
methodsResults <- append(methodsResults, list(additiveMethod(open, period,p)))
methodsResults <- append(methodsResults, list(multiplicativeMethod(open, period,p))) 
methodsResults <- append(methodsResults, list(HoltWinter(open, period, p)))
methodsResults <- append(methodsResults, list(TheilWage(open, period, p)))



t2 = Sys.time()
print(paste('Расчёты заняли:', as.numeric(t2 - t1)))

chosen_parameters = c()
for (i in 1:length(methodsResults)) {
  
  chosen_parameters = c(chosen_parameters, methodsResults[[i]][[p]])
}
#print(methodsResults)
options(digits = 3)

parameter_min = Inf
index = NA
for (i in 1:length(chosen_parameters)){  
  if(parameter_min > chosen_parameters[i]){
  parameter_min = chosen_parameters[i]    
  index = i
  }
}

#bestMethodIndex = which(chosen_parameters %in% min(chosen_parameters))
bestMethodIndex = index
#print(bestMethodIndex)
name1 = c("RandomWalk", "RandowWalkWithDrift", "MeanMetod", "prevMeanMethod", "MovingAverage2", "MovingAverage3", "MovingAverage4", "MovingAverage5", "BrownsLinearTrend", "BrownsSimple", "LinearTrend", "HoltMetod", "AdditiveMethod", "MultiplicativeMethod", "HoltWinter", "TheilWage") 
print(paste('Лучший метод:', name1[bestMethodIndex]))
#print(paste('C', pname, '=', min(chosen_parameters)))
print(paste('C', pname, '=', parameter_min))
print('Forecast:')
print(methodsResults[[bestMethodIndex]][[4]])

plot(open)
plot(methodsResults[[bestMethodIndex]][[4]])

lines(open, type='b', col='blue')
lines(methodsResults[[bestMethodIndex]][[4]], type='b', col='red')
