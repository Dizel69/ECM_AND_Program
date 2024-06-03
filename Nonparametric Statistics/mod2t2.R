#Сравнивается урожайность кукурузы на полях, обработанных разным
#и агротехническими методами. Провести сравнение обработок с помощью критерия
#Краскела–Уоллиса и статистики Имана–Давенпорта. Оценить размер эффекта с помощью статистик θ – Фримана, η^2 и ε^2
#Провести сравнение обработок с контролем (метод 1), используя асимптотический метод

x1<-c(8, 7, 4, 1, 5, 3, 1, 6)
y1<-c(8, 7, 3, 2, 4, 5, 1, 6)
z1<-c(7, 8, 5, 1, 3, 4, 2, 6)
v1<-c(7, 8, 3, 2, 4, 6, 1, 5)


nn<-length(x1)
kk<-4

matrс<-matrix(c(x1, y1, z1, v1), nrow=length(x1), ncol=4, byrow=FALSE)
colnames(matrс)<-c("x1", "y1", "z1", "v1")
rownames(matrс)<-seq(1:nn)


svyaz<-function(x){
  svyazki<-numeric(0)
  rx<-unique(x)
  svyazki<-tabulate(match(x,rx))
  svyaz<-sum(svyazki^3-svyazki)/12
}


tt<-sum(apply(matrс, 2, svyaz))
chisl<-12*sum((apply(matrс, 1,sum))^2)-3*nn*(nn+1)^2*kk^2
znam<-(nn^3-nn)*kk^2-kk*tt

statW<-chisl/znam

#chi^2-аппроксимация - H0: W=0, H1: W>0

statWchi<-kk*(nn-1)*statW
pv<-1-pchisq(statWchi,nn-1)

if(pv<0.05){
  res<-matrix(c(round(statW, digits=3), round(statWchi, digits=3), round(pv,digits=3),  "H1"), nrow=1, ncol=4, byrow=T)
  colnames(res)<-c("Stat", "Norm.stat", "pv", "Decision")
  rownames(res)<-"greater"
  print(res)
}else{
  res<-matrix(c(round(statW, digits=3), round(statWchi, digits=3), round(pv,digits=3),  "H0"), nrow=1, ncol=4, byrow=T)
  colnames(res)<-c("Stat", "Norm.stat", "pv", "Decision")
  rownames(res)<-"greater"
  
  print(res) 
}

#F-аппроксимация, H0: W=0, H1: W>0 

statWF<-(kk-1)*statW/(1-statW)
nu1<-floor(nn-1-0.5*kk)
nu2<-(kk-1)*nu1 
pv<-1-pf(statWF,nu1, nu2)

if(pv<0.05){
  res<-matrix(c(round(statW, digits=3), round(statWF, digits=3), round(pv,digits=3),  "H1"), nrow=1, ncol=4, byrow=T)
  colnames(res)<-c("Stat", "Norm.stat", "pv", "Decision")
  rownames(res)<-"greater"
  print(res)
}else{
  res<-matrix(c(round(statW, digits=3), round(statWF, digits=3), round(pv,digits=3),  "H0"), nrow=1, ncol=4, byrow=T)
  colnames(res)<-c("Stat", "Norm.stat", "pv", "Decision")
  rownames(res)<-"greater"
  
}

alpha = 0.05

alpha1 <- alpha/2
alpha2 <- 1 - alpha/2

ncpmax<-kk*(nn-1)

nc <- seq(0, ncpmax, by = .001)
p <- pchisq(statWchi, (nn-1), nc)

for(i in 1:length(nc)){
  if(abs(p[i]-alpha2)<0.00001){
    resu<-nc[i]
  }
  if(abs(p[i]-alpha1)<0.00001){
    resl<-nc[i]
    print(resl)
  }
}

lowW<- resu/kk/(nn-1)
uppW<-resl/kk/(nn-1)

print(lowW)
print(uppW)