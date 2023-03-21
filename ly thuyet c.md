**I. Phân vùng nhớ trong C**

**1. Phân vùng nhớ**

\- Gồm 5 phân vùng nhớ : Stack, Heap, BSS, DS, Text

\- Text

\+ Tất cả các mã máy trong chương trình sẽ được load vào vùng này

\+ Là vùng chứa lệnh để thực thi trong chương trình

\+ Chỉ có quyền truy cập Read để tránh sửa đổi

\- DS

\+ Có quyền truy cập read-write

\+ Vùng chứa biến toàn cục hoặc static với giá trị khởi tạo khác 0

\+ Được giải phóng bộ nhớ khi kết thúc chương trình

\+ VD :

int x = 1;

static int y = 2; 

\- BSS

\+ Có quyền truy cập read-write

\+ Vùng chứa biến toàn cục hoặc static với giá trị khởi tạo bằng không hoặc không khởi tạo

\+ Được giải phóng bộ nhớ khi kết thúc chương trình

\+ VD :

int x;

stactic int y = 0;

\- Heap

\+ Có quyền truy cập read-write

\+ Được sử dụng để cấp phát bộ nhớ động như Malloc, Calloc...

\+ Sẽ được giải phóng khi gọi hàm free

\- Stack

\+ Có cấu trúc LIFO(Last in, first out) Strucure

\+ Có quyền truy cập read-write

\+ Được sử dụng cấp phát cho biến local, input parameter ...

\+ Sẽ được giải phóng khi ra khỏi block code/hàm

**2. So sánh**

\- Sự khác biệt giữa stack và heap

\+ Giống nhau : đều là vùng nhớ được lưu trữ trong RAM khi chương trình được thực thi

\+ Khác nhau :

+) Stack : 

.Lưu trữ các biến cục bộ trong hàm, tham số truyền vào...

. Truy cập vào bộ nhớ nhanh khi được thực hiện chương trình

. Vùng nhớ được quản lý bởi hệ điều hành, sẽ được giải phóng khi kết thúc chương trình

+) Head

. Lưu trữ vùng nhớ cho các biến con trỏ được cấp phát động bởi các hàm malloc, calloc..

. Vùng nhớ chỉ được giải phóng khi được gọi hàm free, nếu không bộ nhớ sẽ liên tục được tăng lên và có nguy cơ bị tràn

\- Sự khác biệt giữa DS và BSS

\+ Giống nhau 

+) Đều có quyền truy cập read-write

+) Được giải phóng khi kết thúc chương trình

\+ Khác nhau :

+) DS : Là vùng chưa biến toàn cục hoặc static với giá trị khởi tạo khác 0

+) BSS : Là vùng chứa biến toàn cục hoặc static với giá trị khởi tạo bằng không hoặc không khởi tạo

**II. Macro and Funcion**

**1. Macro**

\- Được xử lý bởi preprocessor

\- Thay thế đoạn code được khai báo Macro vào bất kì chỗ nào xuất hiện Macro đó

\- Được khai báo bằng từ khóa **#define**

\- Được ứng dụng để định nghĩa các biến const, hàm …

\- VD : #define SUM(a,b)	(a+b)

**2. Funcion**

\- Là khối lệnh thực hiện một chức năng nào đó trong chương trình

\- Được xử lý bằng compiler khi hàm được gọi

\- Khi hàm được gọi, compiler sẽ phải lưu con trỏ chương trình PC(Pointer Cursor) hiện tại vào Stack,

Chuyển PC tới hàm được gọi, thực hiện hàm đó xong rồi lấy kết quả trả về sau đó quay lại vị trí ban đầu trong Stack trước khi gọi hàm và tiếp tục thực hiện chương trình

**3. So sánh Macro và Funcion**

\- Macro : 

\+ Chỉ thay thế đoạncode Macro vào chỗ được gọi trước khi được biên dịch

\+ Thời gian chạy nhanh nhưng khiến code dài hơn bình thường

\+ Không xác nhận được kiểu dữ liệu của tham số hoặc giá trị nhận được

\+ Không thể debug tìm lỗi trong khi chương trình được chạy

\- Funcion : 

\+ Phải tạo một Funcion call, lưu địa chỉ trước khi gọi hàm

\+ Do phải gọi Funcion call nên tốn thời gian nhưng code sẽ ngắn gọn hơn 

\+ Phải xác nhận được kiểu dữ liệu của tham số hoặc giá trị nhận được

\+ Có thể debug để tìm ra lỗi trong quá trình thực thi

**III.So sánh giữa Struct and Union**

\- Giống nhau : có cách sử dụng và ý nghĩa giống nhau

\- Khác nhau : khác nhau về mặt lưu trong dữ liệu trong bộ nhớ

\+ Struct :  

+) Dữ liệu các thành viên của struct được lưu trữ ở những vùng nhớ khác nhau

+) Kích thước của 1 struct tối thiểu bằng kích thước của các thành viên cộng lại và phụ thuộc vào bộ nhớ đệm (struct padding)

\+ Union :

+) Dữ liệu các thành viên sẽ chung 1 vùng nhớ

\+ Kích thước của Union sẽ được tính bằng kích thước lớn nhất của kiểu dữ liệu trong Union. 

\+ Việc thay đổi nội dung của 1 thành viên sẽ dẫn đến thay đổi nội dung các thành viên khác

**IV. Static**

**1. Định nghĩa :** 

\- Được khai báo bằng từ khóa : ***Static + kiểu dữ liệu***

\- Có phạm vi truy cập trong một file,hàm , không thể truy cập từ file,hàm khác

**2. Biến Stactic cục bộ**

\- Được khởi tạo 1 lần duy nhất và tồn tại trong thời gian chạy chương trình

\- Giá trị không bị mất đi ngay ca khi kết thúc hàm, và mỗi lần hàm được gọi thì giá trị của biến chính là giá trị lần gần nhất được gọi

\- Chỉ có thể được gọi nội bộ trong hàm khởi tạo ra nó

**3. Biến static toàn cục**

\- Chỉ có thể truy cập và sử dụng trong file được khởi tạo nó, các file khác không thể truy cập

**V. Con trỏ**

**1. Con trỏ (Pointer)**

\- Được khai báo bằng cách : kiểu dữ liệu + ‘\*’ 

\- Là những biến được lữu trữ địa chỉ bộ nhớ của những biến khác

\- Do là biến được lưu dữ địa chỉ bộ nhớ nên tất cả các con trỏ đều có kích thước bằng nhau và phụ thuộc vào hệ thống là 16 bit, 32 bit …

**2. Con trỏ hàm** 

\- Kiểu khai báo : type + (\*ptr)(type,…)

\- Là con trỏ lưu trữ địa chỉ của 1 hàm trong bộ nhớ máy

\- Được sử dụng để truy cập vào vị trí của hàm trong bộ nhớ cũng như thực thi các lệnh của hàm

\- Kiểu dữ liệu khai báo và kiểu dữ liệu trong hàm con trỏ phải tương được

\- VD : int (\*ptr)(int x)

**3. Con trỏ void** 

\- Kiểu khai báo : void \*ptr

\- Là con trỏ đặc biệt, khi khai báo sẽ không cần xác định kiểu dữ liệu

\- Có thể lưu trự địa chỉ của bất kì biến nào ( với nhiều kiểu dữ liệu khác nhau) trong chương trình

\- Vì không xác nhận kiểu dữ liệu nên khi sử dụng dể đọc hoặc ghi thì cần phải ép kiểu cho cùng loại dữ liệu muốn đọc hoặc ghi

\- VD : 

int value = 5;

void \*ptr = &value;

uint32\_t \*ptr2 = (uint32\_t \*)(\*ptr)

**4. Con trỏ Null**

\- Là con trỏ không trỏ tới địa chỉ nào cả và có giá trị là 0

\- Ứng dụng : khi khai báo con trỏ mà không biết gán giá trị nào thì ta gán vào giá trị NULL

\- VD : int \*ptr = NULL;

**5. Mảng con trỏ**

\- Khai báo : type \*tên mảng[]

\- Là mảng mà mỗi phần tử đều là một con trỏ và có địa chỉ khác nhau

\- VD : int \*arr[] = { 1,2,3}

**6. Pointer to Pointer** 

\- Kiểu khai báo : \*\*ptr

\- Là loại con trỏ dùng để lưu trữ địa chỉ của con trỏ khác hay giá trị của con trỏ \*\*ptr chính là địa chỉ của \*ptr

\- VD : int \*\*ptr1 = &ptr2


