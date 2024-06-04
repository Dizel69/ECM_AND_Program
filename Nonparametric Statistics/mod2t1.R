#Экспертным методом оценивается конкурентоспособность сливочного
#масла, реализуемого в розничной торговой сети. Опираясь на коэффициент конкордации Кендалла, проверить
#согласованность экспертных оценок. Построить доверительные интервалы для W на основе симуляции и
#нецентрального хи^2–распределения.

x<-c(83, 91, 94, 89, 89, 96)
y<-c(91, 90, 81, 83, 84, 83)
z<-c(101, 100, 91, 93, 96, 95)

r<-3
nx<-length(x)
ny<-length(y)
nz<-length(z)

dat<-c(x,y,z)	
fact=as.factor(rep(c("I", "II", "III"), c(nx,ny, nz)))

df=data.frame(dat,fact)
df

#####Без  связей - точный критерий  

x1<-c(20.6,25.2)
y1<-c(26.3,25.3)
z1<-c(18.5,24)

nx1<-length(x1)
ny1<-length(y1)
nz1<-length(z1)
n1<-nx1+ny1+nz1


kwr<-function(x,y,z){
  nx<-length(x)
  ny<-length(y)
  nz<-length(z)
  ranks<-matrix(rank(c(x, y, z), na.last = "keep"), nrow=3, 
                ncol=max(nx, max(ny, nz)), byrow = TRUE)
  return(ranks)
}

stat<-function(xx){
  
  n<-sum(!is.na(xx))  
  
  fun<-function(x){
    sum(!is.na(x))*(mean(x,na.rm=TRUE))^2
  }
  
  kwstat<-12/n/(n+1)*sum(apply(xx,1,fun))-3*(n+1)
}

ranks1<-kwr(x1, y1, z1)

kwh1<-stat(ranks1)
kwh1

perm <- function(v) {
  n <- length(v)
  if (n == 1) v
  else {
    X <- NULL
    for (i in 1:n) X <- rbind(X, cbind(v[i], perm(v[-i])))
    X
  }
}

nums<-seq(1:n1)
ls<-as.matrix(perm(nums))#матрица возможных перестановок рангов
head(ls, 5)
tail(ls, 5)

stats<-rep(0, nrow(ls))

for(i in 1: nrow(ls)){
  stats[i]<-stat(matrix(ls[i,], nrow = 3,ncol = 2))
}

pv<-sum((stats>=kwh1))/length(stats)
print(pv)

statu<-sort(unique(stats))#определяем уникальные значения

##таблица верхних критических точек
ww<-cumsum(tabulate(match(stats,statu)))/length(stats)#определяем вероятности уникальных значений

wcrit<-matrix(c(rev(ww), ww), nrow=2, ncol=length(ww), byrow=T)#таблица критичеких точек
colnames(wcrit)<-round(statu, digits=3)
rownames(wcrit)<-c("P(T+>=k)", "P(T+<=k)")

print(round(wcrit, digits=3))
#приближённая квантиль уровня alpha=0.1 в таблице в конце pdf файла равна 4.571 


##########Асимптотика
##########ВЫБОРКИ РАВНЫХ ОБЪЁМОВ!!!!!!#############################
##########Для неравных объёмов такой подход не сработает!!!!##########

nx<-sum(!is.na(x))
ny<-sum(!is.na(y))
nz<-sum(!is.na(z))
nn<-c(nx, ny, nz)
n<-sum(nn)

ranks<-kwr(x,y,z)
kwh<-stat(ranks)#статистика без корректировки связей
ru<-unique(c(x,y,z))
svyazki<-tabulate(match(c(x,y,z),ru))

mnoz<-1/(1-sum((svyazki^3-svyazki))/(n^3-n))  #корректирующий множитель с учётом связок 

kwh<-kwh*mnoz#cкорректированная статистика Краскелла-Уоллеса
pv<-1-pchisq(kwh,2)

if(pv<0.05){
  res<-matrix(c(round(kwh, digits=3), round(pv,digits=3),  "H1"), nrow=1, ncol=3, byrow=T)
  colnames(res)<-c("Stat",  "pv", "Decision")
  rownames(res)<-"greater"
  print(res)
}else{
  res<-matrix(c(round(kwh, digits=3),  round(pv,digits=3),  "H0"), nrow=1, ncol=3, byrow=T)
  colnames(res)<-c("Stat", "pv", "Decision")
  rownames(res)<-"greater"
  
  print(res) 
}

print('Что соответсвует')

kruskal.test(dat ~ fact, data =df)


