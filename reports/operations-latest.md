## 1. 작업 목표

해결되지 않은 부모 입력에서 실제 선언된 레코드 길이를 확인하고 정상→비정상 전환의 범위 초과 여부를 구분한다. 부모 ID를 강제로 바꾸기 전에 실제 실패 단계와 수정 가능 범위를 판단한다. 기존 물리 비용 후보와 정상 밀침·낙하·착지·FOLLOW·재등반은 보존한다.

## 2. 실제 수행

이전 attach 경쟁을 피하기 위해 공식 REA의 paused spawn→read-only script load/ready 확인→resume 순서를 사용했다. 기존 공식 stdio 운용 코드를 재사용했으며 provider 설치·등록·보안 설정은 바꾸지 않았다. client 초기 입력 전에 관측기 연결을 확인했고 같은 reader 호출의 entry/parentStart/parentEnd declared byte length·bit capacity·cursor와 setter 인자를 수집했다. 바이트 가드3곳을 통과했고 게임 필드에는 쓰지 않았다.

1논리개체/8파트 하네스와 기존9개 PBO를 사용했다. 전체16표본 수집 후 REA close session·serverexit0·source hash 불변을 확인했다. 원시메모리14byte창과 별도로 선언된 범위를 기록했으므로 mapped memory가 읽힌다는 사실을 유효한 레코드 범위와 혼동하지 않았다. 진단 퇴역 뒤 실제 DayZ 프로세스 부재를 확인한 후 저장했다.

준비 단계에서는 포트 숫자의 단순 치환이 런처 안의 PBO 해시 일부를 바꿔 서버 검증이 거부됐다. 실제 PBO들은 변경되지 않았다. 거부된 런처를 별도 보존하고 해시 상수만 원래 값으로 복구했으며, 소스의 전체64자리 해시 상수를 대조한 뒤 실행했다. 이때는 프로필/게임 프로세스 생성 전이므로 별도 게임 실패로 세지 않는다.

## 3. 측정 결과

|대상|정상 입력8건|비정상 입력8건|
|---|---:|---:|
|declared record bytes|111|16|
|declared bit capacity|888|128|
|부모 필드 시작 bit|657|47|
|부모 필드 종료 bit|659|146|
|부모 필드 소비 bit|2|99|
|선언 범위 초과 bit|0|18|
|부모 ID|0|동일 비정상ID|

비정상 부모 ID64bit는 update/presence2bit 뒤 bit49..112에서 읽혔다. ID 읽기 종료113은128 범위 안이다. pivot은bit113..144에서읽어17bit가범위를넘고, bool은bit145에서읽어누적18bit가범위를넘는다. 따라서 잘못된 ID 자체가 범위 밖 쓰레기 읽기 때문에 생겼다는 설명은 이 표본과 맞지 않는다. 그 뒤의 pivot/bool에서는 선언된 논리 레코드 범위 초과를 실제로 관찰했다.

8개 같은 receiver의 첫 정상 입력과 다음 비정상 입력을 연결했고 raw bits와 인자16/16 일치, read errors0,truncation false다. 이전 별도 초기 writer111byte와 정상 수신111byte의 크기는 일치하지만 같은 packet/entity/time을 대응한 증거는 아직 아니다. 실제 할당 메모리 경계 초과·크래시 원인·CPU 비용으로 확대 해석하지 않는다. 새8/64 기능 결과·시각 품질·Present/CPU/GPU p50/p95/p99·GC 수치는 없다.20ms p95는 목표다.

## 4. 원인 분석

확인: 이전 미확인 길이를 확보해 비정상 레코드의 선언 범위 초과를 입증했다. 하지만 인과 순서는 부모 필드의 잘못된 해석이 먼저이고 pivot/bool 범위 초과가 뒤따른다. setter 이후 변조나 최초 생성부터 잘못된 부모라는 단순 설명은 기존/현재 표본과 맞지 않는다.

미확인/가설:16byte 레코드를 생성하는 쪽의 상태/형식 문제인지, 수신 커서가 앞선 필드를 잘못 해석했는지, 자산/명령 계약이 맞지 않는지 구분되지 않았다. 모델·물리 초기화 옵션을 추측으로 바꾸지 않는다. 직접 검사를 생략하거나 부모 ID0을 강제하면 앞선 입력 불일치가 숨겨질 수 있다.

정적 조사에서는 native input 기록을 큐에 넣는 a7e200과 개체 경로의 직접 caller c6a482,그 상위 c6a1e0에서 c10120을 사용해 기록을 만드는 연결을 확인했다. c0b130 writer의 E8 caller도 좁혔고 기존 c69bb0의 tail jump는 별도다. 이것은 정적 도달 관계이며 실제16byte producer로 확정하지 않는다. encoder c0b130/emitter c6a1e0/queue a7e200의 기존 REA batch 운용을 재사용한 분석을 시작했고, 실행 중 자료는 이번 완료 증거에 섞지 않는다.

## 5. 검증 수준

declared16byte 레코드의18bit 초과와 ID가 범위 내에서 잘못 해석됨: 단일 실행 제한 진단 성공. 원인 확정·모드 수정·8/64 회귀·14클립 시각 품질·성능 반복 검증·최종 수용은 미완료다. 같은 raw자료의독립해석을새실행으로세지않는다.

## 6. 변경 사항

격리 미션/런처/observer/공식stdio진단운용/JSON분석/보고서만 추가했다. 게임 코드·원본 바이너리·기존9PBO·자산·Steam·운영 서버는 변경하지 않았다. 새PBO가없어신규서명대상없음. 기존9PBO의해시·서명검증은이번실행전통과했다. 실제게임은진단완료뒤functional=false·stage0로퇴역했고서버소유정리exit-1을정상게임종료로보고하지않는다.

기존 런처의 observer-armed.json은 확장자와 달리 평문 marker여서 보고서 준비 검증이 이를 거부했다. 원본을 바꾸지 않고 별도 유효 JSON에 내용·길이·해시를 기록해 연결했다. 이를 관측 결과 JSON으로 취급하지 않는다.

## 7. 원시 증거

실행 ID network-gameplay/creature-parent-prearmed1. REA runtime 증거 ev_ca7dd9ab2bcd5b64ff5cd5ff06d7654de2f099d664f6c1318ffe7c11949b5c98. paused spawn/load/resume/status4/close와source hash·16표본·독립 bounds 해석·런처/서명·준비거부/복구소스를저장전해시에연결한다. 공개는집계/상대위치/판정이며rawbuffer·주소·프로세스·로컬경로·게임자산·개인키는제외한다. 실행중 encoder 분석을 중단해 보고서를 쓰지 않았다.

## 8. 잔여 문제

다음 산출물은16byte producer와 decoder의 필드계약을 연결한 수정가능성 판정이다. 공식 모드 API로 생성 조건/명령계약을 바로잡을 수 있는지와 엔진 내부 수정이 필요한지 구분한다. 근거 없이 native reader를 patch하거나 record를 버려서 성공으로 처리하지 않는다. REA 분석의 실제 handle을 확인하며 같은 초기화를 중복 재시작하지 않는다. 비용 OFF/ON은 기존 조건부지지메서드효과와별도로후킹없는8→64전체프레임시험을진행해야하며실제ETW준비는여전히필요하다. 전체목표active.

로컬checkpointSHA256: 62d1861be0345cc12d73b425e00f058aaf2cb08e640b746c157a14b05f2148b2
