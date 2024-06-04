#В средней школе проведены тесты на определение уровня психологической готовности к
# вузовской системе обучения для мальчиков (группа A) и девочек
#(группа B). Исследования отвергли предположение о наличии существенного сдвига
#между выборками. Используя критерий Ансари–Бредли, определите, однородны ли
#эти выборки по степени разброса значений показателя
##########Критерий Ансари-Бредли##########

x<-c(85, 64, 50, 78, 49, 96, 34, 58, 75, 54)
y<-c(65, 78, 93, 55, 70, 90, 80, 79, 82, 67)

###часть выборки без связей  1, 2 и 6-я пары#####

x1<-c(120, 107, 111)
y1<-c(110, 112, 105)

nx1<-length(x1)
ny1<-length(y1)
n1<-nx1+ny1

label=rep(c("male", "female"), each=3) #label - факторная переменная (уровни градации для length)  
length=c(x1, y1)#length - анализируемая числовая переменная
Data1=data.frame(label, length)

ansari_ranks<-function(x){
  n<-length(x)
  r<-rank(x)
  ansari_ranks<-pmin(r, n - r + 1) 
}

a1<-ansari_ranks(Data1$length)
print(a1)

abx<-sum(a1[which(Data1$label=='male')])
print(abx)

ls<-as.matrix(t(combn(n1, nx1)))#матрица возможных позиций рангов для x1  

head(ls, 15)
#tail(ls, 5)

#ранги в нужном порядке
n1<-6
if(n1%%2!=0){
  zz<-seq(1, ((n1+1)/2))
  aa1<-c(zz, rev(zz[1:((n1+1)/2-1)]))
}else{
  zz<-seq(1, (n1/2))
  aa1<-c(zz, rev(zz))
}


stats<-rep(0, nrow(ls))
for(i in 1:nrow(ls)){#вектор возможных значений статистики Ансари-Брэдли
  stats[i]<-sum(aa1[ls[i, ]])#по номерам элементов из ls[i] берём ранги aa1 и суммируем   
}

pv1<-sum((stats<=abx))/nrow(ls)
pv2<-sum((stats>=abx))/nrow(ls)

if(min(pv1, pv2)<0.5){
  pv<-2*min(pv1, pv2)
}else{
  pv<-2*(1-min(pv1, pv2))  
}

print(pv)

statu<-sort(unique(stats))#определяем уникальные значения

##таблица верхних критических точек
abw<-cumsum(tabulate(match(stats,statu)))/length(stats)#определяем вероятности уникальных значений

wcrit<-matrix(c(rev(abw), abw), nrow=2, ncol=length(abw), byrow=T)#таблица критичеких точек
colnames(wcrit)<-statu
rownames(wcrit)<-c("P(T+>=k)", "P(T+<=k)")

print(wcrit)#сравнить с таблицей в pdf файле

#Замечание: верхние и нижние точки связаны - t_low=0.5m(m+n+2)-t_up
#t_up=7 - > P(X>=t_up)=.3
#t_low=0.5*3*(3+3+2)-7=5 -> P(X<=t_low)=0.3



####################Асимптотика ##################
#Создаём фрейм данных##########
nx<-length(x)
ny<-length(y)
n<-nx+ny

label=rep(c("male", "female"), c(nx, ny)) #label - факторная переменная (уровни градации для length)  
length=c(x, y)#length - анализируемая числовая переменная
Data=data.frame(label, length)

#a<-ansari_ranks(Data$length)
#abx<-sum(a[which(Data$label=='male')])

ord<-order(Data$length)
lord<-Data$label[ord]#метка принадлежности ранга 
lenord<-Data$length[ord]#значения по возрастанию.    

if(n%%2==0){
  a<-seq(1:(n/2))
  r<-c(a, rev(a))#ранг Ансари-Бредли без учета связей 
}else{
  a<-seq(1:((n+1)/2-1))
  r<-c(a, (n+1)/2,rev(a))
} 

uniqVals <- unique(lenord)
rcor <- tapply(r, lenord, mean)#ранг Ансари-Бредли с учётом связей 
corected <- data.frame(rcor, uniqVals)
uncor <- data.frame(lenord, lord, r)
dat <- merge(x = uncor, y = corected,by.x = "lenord",by.y = "uniqVals")
print(dat)

####сумма по фрейму данных - функция aggregate
abx<-aggregate(dat$rcor,      #Выбираем столбец
               by = list(dat$lord),#Выбираем индикатор -> порядок по алфавиту  сначала 1-female(Y), 2-male(X)
               FUN = sum)$x[2]     #в методе берём данные по самцам - male(X)

ul<-length(unique(Data$length))

if(n%%2==0){ #объём выборки чётный 
  mabx<-nx*(nx+ny+2)/4#среднее
  if(ul==n){
    dabx<-nx*ny*(nx+ny+2)*(nx+ny-2)/48/(nx+ny-1) #дисперcия без связок
  }else{
    dabx<-nx*ny*(16*sum(dat$rcor^2)-(nx+ny)*(nx+ny+2)^2)/16/(nx+ny)/(nx+ny-1)#дисперcия с учётом связок
  }}else{#объём выборки нечётный
    mabx<-nx*(nx+ny+1)^2/4/(nx+ny)#среднее
    if(ul==n){
      dabx<-nx*ny*(nx+ny+1)*(3+(nx+ny)^2)/48/(nx+ny)^2 #дисперcия без связок     
    }else{
      dabx<-nx*ny*(16*(nx+ny)*sum(dat$rcor^2)-(nx+ny+1)^4)/16/(nx+ny)^2/(nx+ny-1)#дисперcия с учётом связок
    }
  }

abw<-(abx-mabx)/sqrt(dabx)#нормализованная статистика Ансари-Брэдли
pv<-2-2*pnorm(abs(abw))

if(pv<0.05){
  res<-matrix(c(round(abx, digits=3), round(abw, digits=3), round(pv,digits=3),  "H1"), nrow=1, ncol=4, byrow=T)
  colnames(res)<-c("Stat", "Norm.stat", "pv", "Decision")
  rownames(res)<-"two sided"
  print(res)
}else{
  res<-matrix(c(round(abx, digits=3), round(abw, digits=3), round(pv,digits=3),  "H0"), nrow=1, ncol=4, byrow=T)
  colnames(res)<-c("Stat", "Norm.stat", "pv", "Decision")
  rownames(res)<-"two sided"
  
  print(res) 
}



