fn P(v:Vec<i64>){
    let i;
    i=0;
    while i<v.len(){
        print!("{}",v[i]);
        i=i+1;
    }
}
fn main() {
    let x,y;
    x=vec![1,2,3,4,5,6,7,8,9];
    P(x.clone());

}