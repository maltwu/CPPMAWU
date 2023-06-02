
### <font color=#035050>题目</font> 

<font size = 5px face='楷体-简'> 警察局决定从两个帮派<font color=red>青龙帮和白蛇帮</font>开始治理混乱，首先需要确定犯罪分子属于哪个帮派团伙。比如有两名罪犯，他们是否属于同一帮派？需要警察根据不完整的信息作出判断，因为歹徒总是暗中行动的。假设有 $N(N≤10^5)$ 个罪犯，编号 $1\cdots N$ ，其中至少有一人属于青龙帮，至少有1人属于白蛇帮，请依次给出 $M(M≤10^5)$ 个消息，消息类型有两种：D a b 表示a和b属于不同帮派；A a b 表示查询a和b是否属于同一帮派.
</font>

### <font color=#035050>样例</font> 
<font size = 5px face='楷体-简'>**输入：** 第1行包含单个整数T（1≤T≤20)测试用例数量，每个用例由N和M开始；接着是M行，每行包含上述所述形式的一个消息<br>**输出：** 针对每个查询操作，都根据之前收集到的信息进行判断，答案可以为In the same gangs 或 In diffrent gangs和 Not sure yet  
</font>

<table>
<tr border=1px solid black>
    <th>输入样例</th>
    <th>输出样例</th>
</tr>
<tr border=1px solid black>
    <td> 1<br>5  5<br>A  1  2<br>D  1  2<br>A  1  2<br>D  2  4<br>A  1  4<br> </td>
    <td valign=top> Not sure yet<br>In different gangs<br>In the same gangs </td>
</tr>
</table>