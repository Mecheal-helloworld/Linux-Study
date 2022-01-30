### 1、解决 修改/etc/profile 出错后 命令都执行不了问题

​	执行：export PATH=/usr/bin:/usr/sbin:/bin:/sbin:/usr/X11R6/bin

### 2、CentOS 7下maven3.3.9安装

**1.在/usr/local文件中创建**

> mkdir /usr/local/maven

**2.下载maven文件到创建的文件中**

> cd /usr/local/maven
> wget https://archive.apache.org/dist/maven/maven-3/3.3.9/binaries/apache-maven-3.3.9-bin.tar.gz

**3.解压 apache-maven-3.3.9-bin.tar.gz 文件**

> tar -zxvf apache-maven-3.3.9-bin.tar.gz

**4.设置maven的环境变量**

> vi /etc/profile

**5.在profile的文件末尾加上以下配置**

```
export M2_HOME=/usr/local/maven/apache-maven-3.3.9
export PATH=$M2_HOME/bin:$PATH
```

**6.执行source命令让新配置的profile生效**

> source /etc/profile

**7.测试maven是否安装成功**

> mvn -vApache Maven 3.3.9



export JAVA_HOME=/usr/java/jdk1.8.0_171
export CLASSPATH=$JAVA_HOME/lib/
export PATH=$PATH:$JAVA_HOME/bin

export PATH JAVA_HOME CLASSPATH