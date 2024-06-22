#Инициализация и начальные настройки
rm(list=ls())  
step <- 0.1
border <- 20
numequations <- 4
epsilon <- 0.00001
coefficients <- matrix(NA,nrow=numequations,ncol=numequations)

#Функция проверки альфа-доминантности
check_alpha <- function(matrix){
  alphas <- rep(NA,numequations)
  for ( i in 1:numequations){
    alphas[i] <- sum(abs(matrix[i]))
  }
  alpha <- max(alphas)
  result <- list(alpha = alpha,sign = NULL)
  if(alpha<1){
    result[["sign"]] <- (TRUE)
  }
  else {
    result[["sign"]] <- (FALSE)
  }
  return(result)
}

#Функция проверки бета-доминантности
check_beta <- function(matrix){
  betas <- rep(NA,numequations)
  for ( i in 1:numequations){
    betas[i] <- sum(abs(matrix[,i]))
  }
  beta <- max(betas)
  result <- list(beta = beta,sign = NULL)
  if(beta<1){
    result[["sign"]] <- (TRUE)
  }
  else {
    result[["sign"]] <- (FALSE)
  }
  return(result)
}

#Функция проверки гамма-доминантности
check_gamma <- function(matrix){
  gammas <- rep(NA,numequations)
  for ( i in 1:numequations){
    gammas[i] <- sum(matrix[i,i]*matrix[i,i])
  }
  result <- list(gamma = gamma,sign = NULL)
  if(gamma<1){
    result[["sign"]] <- (TRUE)
  }
  else {
    result[["sign"]] <- (FALSE)
  }
  return(result)
}

#Инициализация системы уравнений
coefficients <- matrix(c(-2.77,-0.04,0.21,-0.18,
                         0.26,-2.76,0.11,0,
                         0.41,-0.23,-2.94,0,
                         0.05,-0.26,0.36,0.88),nrow=numequations,ncol=numequations,dimnames = list(c("1","2","3","4"),
                                                                                                   c("x1","x2","x3","x4")))
constants <- rbind(c(-1.24,-0.62,0.88,1.17))

system <- list(matrix = coefficients,constants =constants )

#Функция проверки строк на диагональное доминирование
check_string_dominant<- function(vector,index){
  sum <- sum(abs(vector))
  sum <- sum - abs(vector[index])
  if(abs(vector[index])>sum){
    return (TRUE)
  }
  return (FALSE)
} 

#Функция проверки матрицы на диагональное доминирование
check_matrix_dominant<- function(matrix){
  for(i in 1:numequations){
    if(check_string_dominant(matrix[i,],i)== (FALSE)){
      #print("No diagonal dominant")
      return (FALSE)
    }
  }
  return (TRUE)
} 

Функции для перестановки строк и создания диагонально доминирующей матрицы
swap_string <- function(system,i,j){
  copy <- system
  buff_matrix <- copy$matrix[i,]
  copy$matrix[i,] <- copy$matrix[j,]
  copy$matrix[j,] <- buff_matrix
  
  buff_constants <- copy$constants[i]
  copy$constants[i] <- copy$constants[j]
  copy$constants[j] <- buff_constants
  
  return(copy)
}

make_string_dominant <- function(system,i){
  res <- list(matrix=NULL,constants=NULL)
  if(check_string_dominant(system$matrix[i,],i)==(TRUE)){
    res$matrix <- system$matrix[i,]
    res$constants <- system$constants[i]
    return(res)
  }
    for(alpha in (seq(-border,border,by=step))){
      for(beta in (seq(-border,border,by=step))){
        for(j in 1:numequations){
          if(alpha==0 || beta == 0){
            next
          }
          if(i==j){
            next
          }
          res$matrix <- alpha * system$matrix[i,] + beta * system$matrix[j,]
          if(check_string_dominant(res$matrix,i)==(TRUE)){
            cat("alpha = ",alpha," beta = ",beta," String ",i,"+",j,"\n")
            res$constants <- alpha * system$constants[i] + beta * system$constants[j]
            return(res)
          }
        }
      }
    }
  return(FALSE)
}
make_diagonal_dominant <- function(system){
  if(check_matrix_dominant(system$matrix)==(TRUE)){
    cat(" Matrix has diagonal dominant","\n")
    return(system)
  }
  result <- list(matrix=NULL,constants=NULL)
  for( i in 1:numequations ){
    funcres <- make_string_dominant(system,i)
    if(class(funcres)=="logical" && funcres == (FALSE)){
      cat("Matrix  cant led to diagonal dominant, string = ",i,"\n")
      return(FALSE)
    }
    result$matrix <- rbind(result$matrix,funcres$matrix)
    result$constants <- c(result$constants,funcres$constants)
  }
  cat(" Matrix led to diagonal dominant","\n")
  return(result)
}
print(system)
#Проверка системы уравнений
answer <- make_diagonal_dominant(system)


#Нормализация и решение системы уравнений
new_system <- answer$matrix
new_system <- cbind(new_system,c(NA,NA,NA,NA))
for(i in 1 : numequations){
  for(j in 1 :numequations){
    new_system[i,j] <- answer[["matrix"]][i,j] / answer[["matrix"]][i,i]
  }
  new_system[i,numequations+1] <- answer[["constants"]][i] / answer[["matrix"]][i,i]
}

for(i in 1 : numequations){
  for(j in 1 :numequations){
    if(i!=j){
    new_system[i,j] <- (-1) * new_system[i,j]
    }
    if(i==j){
      new_system[i,j] <- 0
    }
  }
}

A <- new_system[1:numequations,1:numequations]

alpha_res <- check_alpha(A)
alpha <- alpha_res[["alpha"]]
beta_res <- check_beta(A)
beta <- beta_res[["beta"]]
#gamma_res <- check_gamma(A)
#gamma <- gamma_res[["gamma"]]

 ro <- max(abs(new_system[,numequations+1]))
# ro <- sum(abs(new_system[,numequations+1])) beta
# ro <- sqrt(sum(new_system[,numequations+1] * new_system[,numequations+1]))
n <- log(epsilon*(1-alpha)/ro)/log(alpha)
#n <- log(epsilon*(1-beta)/ro)/log(beta)
#n <- log(epsilon*(1-gamma)/ro)/log(gamma)


n <- ceiling (n)


l <- matrix(new_system[,numequations+1],nrow=numequations,ncol=1)

for(i in 1:n){
  l <- A%*%l
  l <- l + new_system[,numequations+1]
  cat(i,": ",l,"\n")
}

r <- answer[["matrix"]] %*% l - answer[["constants"]]
print(r)

#Эти шаги выполняют нормализацию матрицы, проверяют 
#условия сходимости и решают систему уравнений методом 
#итераций. Выводится решение и проверка результата.

