## 1. 작업 목표

같은 개체의 부모 ID0→비정상ID 전환에서 잘못된 값이 setter 이후 생겼는지, setter 전에 읽은 비트열에 이미 있었는지 구분한다. 추가로 입력 길이와 실제 송신 생성 경로를 확인해 국소 수정 가능성을 판단한다.

## 2. 실제 수행

기존1개체8파트 하네스로 별도 두 실행을 진행했다. 첫 실행은 client c035e0 진입·부모 비트 읽기 직전 c041ad·setter b59610을 같은 thread/receiver로 묶어 관측했다. 원본 명령 byte guard를 사용하고 게임 필드에는 쓰지 않았다.14byte 로컬 메모리 창은 전체 패킷 캡처가 아니며 이때 논리 길이를 확보하지 못했다. 읽은 비트열을 MSB-first flags 및 bytewise little-endian ID/pivot으로 독립 해석해 실제 인자와 대조했다.

두 번째 실행은 declared byte length/bit capacity 필드를 추가하고 server 실제 writer slot8 c69bb0를 관측했다. CLI가65000ms 요청을 최대60000ms 규칙으로 거부한 결과를 보존하고60000ms로 변경했다. 서버16표본은 확보했지만 client listener는 초기 입력을 놓쳐 calls0/matches0이다. 이 실행을 길이 검사 성공으로 보고하지 않는다. 같은 attach를 반복하는 대신 다음에는 초기 입력 전 observer를 장착해야 한다.

원본 A500 파일과 MSVC unwind 연결로 writer c69bb0→c0b130, state 전달 a7eda0/a7ec90의 호출·BitStream 생성 필드를 제한 대조했다. a7ec90의 수신 생성에서는 vector byte count와 count*8을 stream +8/+c에 기록한다. 이것은 해당 코드의 정적 증거이며 이번 미포착 stream의 실제 길이가 아니다.

## 3. 측정 결과

|대상|결과|
|---|---|
|첫 실행 input receiver|8개,각 정상/비정상 한 번씩16표본|
|독립 비트 해석과 setter 인자|16/16 일치|
|정상 parent update|presence0,2bit 소비,prefix114bit|
|비정상 parent update|presence1,99bit 소비,prefix19bit|
|비정상 parentId|8/8 동일 dfbf3c0de39c400d|
|두 번째 server writer|8개 receiver,각2회=16표본|
|저장된 writer payload|모두111bytes,883written bits|
|두 번째 client declared bounds|0표본,미확인|
|새 프레임/시각 측정|없음|

첫 실행의 정상 입력 entry는 byte67 bit7,부모 시작 byte82 bit1,종료 byte82 bit3이다. 비정상 입력 entry byte3 bit4,부모 시작 byte5 bit7,종료 byte18 bit2이다. 소비99bit를 raw window로 해석한 ID/pivot/bool과 실제 인자가 모두 일치했다. 비정상 pivot은 같은 값만이 아니라 여러 값으로 관찰됐다. 이것만으로 heap overread라고 확정하지 않는다.

두 번째 초기 writer의 byte82 창은 첫 실행 정상 parent 창과 일치한다. 정상 flag+64bit 패턴은 writer bit657에 있지만 비정상 패턴은 저장된 초기 writer buffer에서 발견되지 않았다. 서로 다른 실행의 패턴 검색이며 packet/entity/time 대응이 아니다. 처음2회 이후의 writer 내용은 수집하지 않았으므로 송신 측이 항상 정상이라고 일반화하지 않는다.

## 4. 원인 분석

확인: setter 진입 이후의 임의 메모리 변조 설명은16표본과 맞지 않는다. 잘못된 값은 해당 bit cursor에서 읽은 비트열과 일치한다. 서로 다른 prefix 길이와 parent presence 분기를 확인해 입력 레코드 형식/상태 전환/커서 계약을 다음 조사 대상으로 좁혔다.

가설: delta/full-state 계약 불일치, 앞선 필드 해석의 정렬 차이, 입력 길이 초과 가능성이 남는다. 비정상 레코드에서 byte18 bit2까지 진행한 사실만으로 범위 초과를 선언할 수 없다. 실제 declared length와 같은 실행의 producer mapping이 필요하다. 기존 생성 시 부모없음 상태가 확인된 사실도 유지한다.

보류: ID0 강제, 경고 필터, ECE_INITAI 추가, 물리 비활성화, 새로운 보간/시계 변경은 시행하지 않았다. 이 관측은 네트워크 오류와 연결되며 과거 경고 없던 오프라인64개체 고비용의 원인을 증명하지 않는다.

## 5. 검증 수준

부모 비트→인자 대응: 제한 진단 성공. 초기 native input writer 실행: 진단 성공. declared bounds/packet end-to-end 대응: 미확인. 두 실행은 서로 다른 관측조건이며 같은 수정의 반복 성공으로 세지 않는다.8/64 기능 회귀·전체14클립 품질·성능·최종 수용은 미완료다.20ms p95는 목표다.

## 6. 변경 사항

격리 미션·읽기 전용 스크립트·정적 JSON·관측/해석 JSON·보고서만 추가했다. 원본/게임 코드/9개 PBO/자산/Steam은 보존했다. 기존9PBO는 각 서버 런처의 해시·서명 검증을 통과했고 신규 PBO/서명 대상은 없다. 첫 실행은 diagnostic stop에 따라 functional=false로 퇴역했다. 두 번째는 start gate 대기 제한에 도달해 completedMarker 없이 종료됐다. 이후 작성한 stop flag가 실제 종료 원인이었다고 하지 않는다. 실제 프로세스 부재와 두 관측기 종료0/cleanup null을 확인했으며 정상 게임 종료를 보장했다고 보고하지 않는다.

## 7. 원시 증거

묶음 ID network-gameplay/parent-bitcursor-and-writer1. 첫 reader ev_f2083efc8553aada6de04c3cb863e9a20d6b7b75ab0492b421d3a0e8e21d7e72,server writer ev_46014844aa6598769cacf0253f99a3983b0d796c087e11adfcc6c0c81f8a7691,미포착 client ev_c0b62f10044e3282a32ee133a91e190c3833e625921efa08ed45a1d06997343a. plans/scripts/raw JSON/독립 해석/정적 unwind/launch·signaturechecks/거부 기록을 로컬 해시에 연결한다. 공개는 집계와 상대 위치만 담고 raw buffer·주소·개인키·프로세스·로컬 경로를 제외한다.

## 8. 잔여 문제

다음 최소 시험은 초기 입력 전에 observer를 장착해 첫 정상/다음 비정상 레코드의 declared bounds를 확보하는 것이다. attach 경쟁으로 놓친 시험을 같은 타이밍으로 반복하지 않는다. 실제 producer도 초기2표본만이 아닌 첫 상태 변경을 구분할 조건이 필요하다. 원인이 모드 계약에서 수정 가능한지 확인한 뒤에만 국소 후보·위험·기능 검증 비용을 제시한다. 비용 OFF/ON은 후킹 없는 새 baseline 및 실제 ETW 관리자 도우미 준비 후8→64로 진행한다. 전체 목표 active.

로컬checkpointSHA256: 305edb565b6d2857b95861bb657268dd37ff41c2f6ac3260fafb7736f54ccbb5
