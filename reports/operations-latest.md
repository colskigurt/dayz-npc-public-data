# 일반 Animate 경로 적격성:실제 선택 객체의component null 관측

작성: 2026-10-31T09:42:10.551625+09:00

## 1. 작업 목표

일반 IEntity.Animate/AnimateEx가 현재 NPC의 스켈레톤 시계 제어 후보가 될 수 있는지, 기존 정적 handler 해석과 실제 Native Animal 객체의 수신 경로를 대조한다. 원인 확인 전 시간 주입·보간·업데이트 생략을 적용하지 않는다.

## 2. 실제 수행

설치된 scripts.pbo에서 IEntity.Animate(float speed,int loop) / AnimateEx(float speed,int loop,out vector lin,out vector ang),GetVObject 선언과 Object→IEntity·ObjectTyped·Entity·EntityAI·DayZCreature·DayZCreatureAI·DayZAnimal 상속 선언을 추출했다. speed라는 선언을 dt로 바꾸어 해석하지 않았다.

기존 Capstone handler는 entity+0x60가 null이면 가상 호출 없이 반환하며, null이 아니면 component+0x10의 vtable+0xa8로 전달한다. 해당 정적 자료의 저장 해시를 대조했다. 새 Ghidra 전체 분석은 실행하지 않았다. 앞선900000ms startup timeout을 같은 조건으로 재시작하지 않았다.

실제 클라이언트에서120초 고정 run clip2,논리1개체/네이티브8파트,AI·루트 이동OFF를 실행했다. 읽기 전용 query/read 마스크0x410로 Native Animal 후보를 찾고,73본의 유일 후보에서 generic 경로를20초 표본화했다. 원래 probe의 값은 유지하고 해당 경로 조회만 추가했다. 기존 renderer owner ancestry945표본 null 경로의 반복이 아니라 Native Animal 루트에서 시작한 조회다. hook·메모리 쓰기·속도 수정·새PBO는 없다.

화면에서 고정 개체와 HUD를 확인하고 frame1513의 정지 화면을 보존했다. 이번에는 동영상을 녹화하지 않았다. launch의 visualCapture=true는 HUD 계측 플래그이며 실제 녹화 완료를 뜻하지 않는다.

## 3. 측정 결과

- 새 실행 runtime-20261031-093649, 정상 종료, 반복1회.
- 읽기 표본 1840, 선택 후보 기록 1840개. entity+0x60 null 1840개.
- 두 번 읽은 객체 헤더가 같은 기록 **1839개**도 모두 null. 비안정1개는 안정 표본 판정에서 제외했다.
- 읽을 수 있는 component+0x10/slotA8 체인 **0개**. Native RTTI는 DayZAnimal,subobject offset0이나 실제 스크립트 호출의 rcx는 관측하지 않았다.
- 읽은 Native Animal 후보들의 primary interval50ms·secondary200ms. 스크립트 콜백 약12.83592Hz,스크립트 nativeSeconds/missionSeconds 0.641790.
- 15~55초 mission 전경 비율 1.0,p50/p95/p99 **20.0/35.0/37.0ms**,루트 보정0·최대drift0. 메모리 스캔·10ms 표본화가 있는 진단 조건이므로 비용 개선이나64개체 성능으로 사용하지 않는다.
- 메모리 스캔은 약5.328GiB를 읽었고 16.867초 걸렸다. 그 용량 전체를 저장한 것이 아니다.

## 4. 원인 분석

확인: 현재 선택한 Native Animal 루트에는 관측 구간의 generic component가 없었다. 스크립트 상속과 정적 native RTTI만으로 handler가 받는 포인터가 이 주소라고 확정할 수는 없다. 따라서 **이 객체가 handler 수신자라면 null 분기에서 끝난다**는 조건부 결론이다. Animate가 모든 NPC에서 항상 불가능하다는 전역 결론은 아니다.

판정: 현재 경로를 스켈레톤 시계의 지원 제어로 채택할 근거는 확보되지 않았다. float speed를 누적 dt처럼 주거나 존재가 입증되지 않은 그래프 시간 변수를 주입하지 않는다. 실제 스크립트 호출·가상 메서드 소비·그래프 시계 변화는 미관측이다.

기존 사실과 구분: 과거 계측에서50ms 소비·누적기 초과분 손실과 CPU/영상의 포즈 증분 연결이 관찰됐지만,이번 표본은 새 graph-clock 소비 trace나 새 영상 증거가 아니다. 스크립트 누적 시계0.64를 그래프 phase 또는 표시 FPS로 바꿔 부르지 않는다. 고정50ms interval만으로 모든 시각 끊김의 원인을 설명하지 않는다.

## 5. 검증 수준

- 설치 API/상속·기존 handler 자료 해시·실제 선택 객체 경로: 진단 성공,이번1회 범위.
- 지원 시계 제어 경로: 미확보. 후보 배포/효과 검증 미실행.
- 실제 표시의 시간적 품질·전체14클립·8/64 기능·64비용·20ms p95·전체 골: 미완료.

## 6. 변경 사항

시험 빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185의 기존 격리 clip launcher를 사용했다. 최신 CBFB 기능 observer나 운영 working D61D로 혼동하지 않는다. 기존 게임 소스·PBO·씬·운영 서버 변경 없음. 새 파일은 bounded read-only probe·watcher·API/RTTI 감사·분석·보고서다. 실행 전 의존PBO3개 서명 검증을 통과했고 새빌드/서명은 없다. 개인키·과거 원시 기록은 보존했다.

## 7. 원시 증거

plan.json / installed-api.json / native-root-rtti.json / results.json,profile의 launch.json·native-policy-memory.jsonl·native-clock-bridge.jsonl·native-clock-probe-control.json·probe 소스 snapshot·visible-frame-clock.tsv·controlled-analysis.json을 연결한다. 관측기 SHA-256과 실제 실행 snapshot을 대조했다. 기존 eligibility-observations.json에 명시된9개 정적 입력 해시도 재검증했다. frame1513 정지 화면은 개체/HUD 확인 자료이고 시간적 부드러움 증거가 아니다. 원시 전체·실행 주소·로컬 경로·프로세스 정보는 공개본에서 제외한다.

## 8. 잔여 문제

generic null 경로와 기존 빈 renderer owner ancestry를 새 입력 없이 반복하지 않는다. 다음은 실제 skeletal evaluator에 도달하는50ms 갱신 경로에서 시간 소비와 reset/보간 세대 계약을 연결하고,지원되는 개입 지점인지 먼저 확인하는 것이다. 실제 receiver/소비 단계가 없는 일반 API 조언을 바로 적용하지 않는다.

지원 제어가 확보되면 한 요소만 바꾼 고정1개체ABBA와 실제 영상에서 평균 속도·세대 경계 포즈 증분·held frame을 분리한다. 평균속도만 정상이고 경계 점프가 악화되면 기각한다. 현재는 그 대조를 실행할 유효 후보가 없으며 미실행으로 남긴다.64 기능/비용 실험을 이 메모리 관측과 합쳐 원인을 귀속하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `77d8cb36d8a894ff4c8c1bfbb86d195a8bbd40c880aa3c1f43bfcca8ea9efe76`. 원시 전체가 아니다.
