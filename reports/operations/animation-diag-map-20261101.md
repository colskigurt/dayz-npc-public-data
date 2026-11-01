# 진단 빌드의 애니메이션 호출·객체 배치 정적 매핑

작성: 2026-11-01T17:24:24.273437+09:00

## 1. 작업 목표

연결 기준 시험을 통과한 진단 exe에서 네트워크 애니메이션 계측 주소와 필드 소유자를 별도로 확인한다. 일반 exe의 주소·객체 배치를 검증 없이 재사용하지 않는다. 기존 게임 구현과 시간 손실·표시 연구를 보존한다.

## 2. 실제 수행

기존 PE RTTI·Capstone 분석 도구를 한정된 함수/직접 분기 창에 사용했다. 실행·부착·패치 없이 파일을 읽었다. 각 원시 명령 바이트·범위·해시·외부/간접 경계를 저장했다. 이는 REA native provider의 완전한 decompilation/runtime Evidence가 아니다. 기존 provider 전체 분석 실패를 이유 없이 반복하거나 새 도구를 설치하지 않았다.

DayZAnimal RTTI의 primary/subobject 표를 각각 찾고, 동일 primary slot 후보를 실제 명령으로 확인했다. buffer subobject의 반환-owner thunk·slot0x48 소비 함수·일반 dispatcher·scheduler의 직접 호출·mode 분기를 연결했다. descriptor의 numerator/duration과 joint-transform 보간 경로 및 API query 경로를 별도 확인했다.

## 3. 측정 결과

입력은 일반 C6709ECE5468FADEB7EDA085355A7DC5157601DDC589FB2BEAC6F5726A99F933, 진단 A500BE804FFED5CDE369337AEB6AB477EF5EE0B5015764BE2B1953C85EC3060D다. primary 표에서 연속 code-pointer prefix는365/369개로 달랐다. 완전한 class ABI 동등성 판정은 아니다.

진단 객체의 animation context는+0x628, mode는+0x670, mode0 누적기는+0x674, buffer subobject는+0x600이다. 일반 exe의 대응 근거는 각각+0x620/+0x668/+0x66c/+0x5f8였다. 진단 mode0 threshold는+0x258에서 읽는다. secondary interval과 graph-clock 필드는 아직 독립적으로 매핑하지 않았다.

진단 RVA: scheduler C6A020, dispatcher A7E7A0, descriptor consumer C680A0, item consumer C69700, interpolation C08340, joint state interpolation C083D0, MS query wrapper C67B00/helper C04760, hash-to-slot310320이다. scheduler97명령, dispatcher257명령, 선택 interpolation79명령(명시 창 안의 직접 tail 경로 포함), joint helper148명령을 보존했다. 숫자는 런타임 호출 횟수가 아니다.

새 runtime/8·64 회귀/frame-time p50/p95/p99/CPU·GPU·표시/할당·GC 측정은 없다.20ms p95는 목표다. 새 PBO·영상·덤프·클라이언트 실행은 없다.

## 4. 원인 분석

확인된 사실: 진단 scheduler의 mode2는 entity+0x600을 dispatcher에 넘긴다. dispatcher는 subobject+0x18에 dt를 더하고 처리한 interval을 빼며, slot0x48이 retained descriptor 소비로 이어진다. mode0에는 누적기+0x674를0으로 초기화하는 분기가 남아 있다. 이는 정적 구조이며 진단 live mode나 개선 효과의 확인은 아니다.

확인된 사실: descriptor 소비는 animation context를 sub+0x28에서 읽고 interpolation에 넘긴다. interpolation은 numerator를 retained+0x38 duration으로 나누며 context+0xA0의 joint helper를 호출한다. helper는 그 상태+0xF0, 즉 context+0x190의 skeleton에서 +0x30 배열에32-byte transform slot을 쓴다. query MS helper는 context+0x188의32-byte transform을 읽고 matrix로 변환한다.

따라서 압축 transform 갱신, cached matrix 조회와 실제 표시를 구분해야 한다. 이전 일반 빌드의 matrix 관찰을 소급 무효화하거나 반대로 이번 진단 배치를 그대로 대입하지 않는다. cache 생성·무효화와 일반 빌드 대응은 추가 확인 대상이다.

미확인: 진단 context+0x198을 graph clock으로 사용할 근거는 아직 없으므로 읽기 계획에서 제외한다. diag-pose.json의 poseAndClock이라는 중간 라벨은 후보명이며 C08660 원문은 root-transform 보간을 보여준다. graph-clock writer를 확보했다고 취급하지 않는다. 전부8byte 이동한다는 일반 규칙도 만들지 않는다.

## 5. 검증 수준

선택 정적 호출·필드 관계 매핑은 진단 성공이다. 진단 live object/call 계측은 미실행이다. 일반 exe 수정, 전체 기능 회귀·반복 수용·애니메이션 부드러움·성능 최종 수용은 미완료다.

## 6. 변경 사항

기존 research에 SHA-bound 정적 분석 생산자와 결과만 추가했다. 분석 모듈의 target/hash는 해당 Python 프로세스 메모리에서만 바꿨고 공용 도구 원문이나 게임/PBO/ANM/9350 후보/운영 서버는 변경하지 않았다. 새 PBO가 없어 새 서명 대상은 없다. 직전 진단 기준 시험에서 기존3개 PBO 서명 검증은 성공했으며 이번에는 게임을 실행하지 않았다.

## 7. 원시 증거

분석 ID animation-diag-map-20261101-1720. diag-map/paths/callers/clock/pose/interpolation-state 및 생산자.py를 보존한다. direct-call byte 후보는 명령 해독·unwind 소유 범위와 대조했다. unwind가 함수 fragment일 수 있음을 표시했다. 본문·선정 evidence는 외부 호출을 재귀적으로 전부 검증했다는 주장이 아니다.

공개본에는 정적 RVA/필드·선정 함수와 한계만 포함한다. 실행 중 ASLR 주소·프로세스·CLI·전체 바이너리/게임 자산·키·원시 영상은 없다. 입력 해시·JSON·증거 해시·파일 안정성과 종료 상태를 확인하고 기존 체크포인트를 갱신했다.

## 8. 잔여 문제

다음은 검증된 진단 fixture에서 이 SHA-bound 함수4개와 객체/스켈레톤 조건을 사용한 bounded 읽기 전용 trace1회다. 잘못된 vtable·mode·shape면 중단한다. graphClock은 writer가 확인될 때까지 unavailable로 남긴다. compressed pose를 cached matrix와 구분해 기록하고 이후 실제 표시 관찰과 연결한다.

진단 결과가 나오더라도 일반 exe의 별도 검증을 대신하지 않는다. mode0 손실 원인, 추출/변환 대조,64개체 기능/비용은 유지하며 새 보간·시간 배율·충돌 검사 생략을 적용하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `068bc7495cefea1b44e6e7c745355fb45e51b949662e9b139db890a7e4ff542a`. 원시 전체가 아니다.
