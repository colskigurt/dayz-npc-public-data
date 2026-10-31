# 실행용8파트 자산·기존 native 포즈 연결 검증,64 ETW 도우미 준비

작성: 2026-11-01T07:09:03.105852+09:00

## 1. 작업 목표

대표 달리기의 root-motion TXA에서 실제8파트 skeleton subset·ANM·패키지·과거 native 본 관찰까지 근거를 연결한다. CPU 포즈를 실제 표시 품질로 확대하지 않는다.64개체 비용 측정 도우미는 게임 변경 없이 별도 준비한다.

## 2. 실제 수행

root TXA와8개 subset의 남은 이름·부모·로컬 키 토큰을 직접 대조했다. 컴파일 기록의 ANM 해시를 현재 파일·실제 PBO 내부 바이트와 대조하고 인스턴스의 run 연결을 확인했다. AddonBuilder의 작은 config.bin은 공식 CfgConvert로 연구 폴더에만 풀어 실제 integration의 subset/graph 연결을 확인했다. 새PBO는 만들지 않았다.

기존 client-20261027-010107의 달리기 같은 프레임 비교 trace를 해시와 행/완전8파트 표본 구성부터 다시 확인했다. 기존 script/CPU 본 질의 자료를 사용했고 새 게임 시험이나 영상은 생성하지 않았다.

최초 검사기는 공용 integration D61D를 실제 시험용7E16과 같다고 잘못 가정하여 해시 gate에서 중단됐다. 파생 검증기는 보존된 mission engine-compile의 실제 격리 release 경로를 읽고 launch 해시를 강제했다. 공용·격리 빌드 모두 보존했다. config.cpp 대신 config.bin이 포장된 것도 파생 판독기로 처리했고 최초 실패 검증기를 보존했다.

별도 일회용 ETW worker는 과거 도우미의 콘솔/Transcript 의존을 제거하고 도구 시작 전·후·timeout·error를 파일에 기록하도록 준비했다. 기존 관리자·PID/프로필/빌드/신선한 LOW_HOLD 검사와30초 단일 측정 제한은 유지했다. 실제 PresentMon --help 자식 실행·종료 시험과 비관리자 실행 거부를 확인했다. 도움말은 stderr UTF16에 쓰고 exit1로 끝나므로 검증기의 초기 exit0/빈stderr/UTF8 가정은 수정했다. 도움말 시험을 재측정이나 ETW 성공으로 세지 않았다. 사용자에게 새 바탕화면 바로가기와 직접UAC 승인을 요청했으며 승인을 추정하지 않는다.

## 3. 측정 결과

8파트 본 수는65/69/65/68/71/73/74/50, 합계535(전체 골격8개2000본 대비)다. 대표run의535개 retained node 이름·부모·키 토큰은 원본 root TXA와 정확히 일치했다.8개 compiled ANM은 과거 컴파일 감사와 현재 PBO 내부 바이트가 일치했고, 원본 full root ANM도 일반 PBO 내부와 일치했다.535/2000은 골격 구조 수치이며 비용 절감률이 아니다.

기존 run trace1845개 완전8파트 표본·14760행, retained 본 비교987075회,4개 transform vector 비교3948300회와 기록 해시가 일치했다. warmup후 최대 본 위치차·transform vector차·nativeSeconds 차이는 모두0이다. 과거 시험과 현재 원본453D·subset0C6E 패키지 해시가 동일함을 확인했다.

새 ETW worker의 도움말 자식 프로세스는 시작·정상 종료 기록을 남겼고, 비관리자 production 호출은 ready/request나 게임을 만들지 않고 거부됐다. 실제64 ETW 자료는 아직 없다. 새8/64 안정성·프레임 p50/p95/p99·GPU/Present 값은 미측정이다. 앞선64/64·92/92·script25/58/78ms 기록을 유지하고20ms p95 목표를 달성했다고 주장하지 않는다.

## 4. 원인 분석

사실: 이 대표run에서 subset pruning·파일 포장·리소스 바인딩 때문에 키가 바뀌었다는 증거는 발견하지 못했다. 과거 CPU 본 질의에서는 전체/축소 골격이 동일한 프레임 변환을 보였다.

한계: 같은 compiler가 양쪽 ANM에 공통 오류를 만들면 전체/subset 비교로 발견하지 못할 수 있다. ANM decoder로 원시 채널을 독립 대조한 것은 아니다. CPU 질의가 같아도 GPU skinning·화면 갱신·원본 게임 포즈가 같다는 증거는 아니다. 과거 얼굴 트랙 제외·최초 추출 독립 검증의 한계도 유지한다.

ETW: 과거 요청접수 후 tool-start 앞에서 멈춘 정확한 원인은 미확정이다. console/Transcript 출력 대기는 가설이다. 새 worker의 실행 경계 기록과 console 분리는 재현 시 구간을 가르기 위한 준비이며 원인 해결이나 ETW 수집 성공을 입증하지 않는다.

## 5. 검증 수준

대표run 자산 provenance·기존 trace 재검증은 진단 성공. 과거 전체/subset 같은 프레임 본 변환 일치는 해당 시험 범위만 통과. 새 ETW 도우미 수명 및 권한 거부 시험 통과. 최초 추출·compiled ANM 독립 decode·실제 화면·64 CPU/GPU 비용·20ms·최종 수용은 미완료다.

## 6. 변경 사항

읽기 전용 chain 분석기·파생 보고서와 별도 ETW 도우미/요청기만 추가했다. 게임 코드·PBO·기존 도구/검증 실패본·원시 실험·공용D61D·운영 서버를 변경하지 않았다. 새PBO가 없어 새 서명 대상은 없다. 실제 시험은 기존 검증된7E16 격리 빌드를 그대로 사용하도록 준비했다.

## 7. 원시 증거

animation-runtime-chain-20261101-0703의 chain-results, audit_chain/v2/v3, derivation, config-decode 및 기존 client-20261027-010107의 launch/subset-compare.tsv와 컴파일·subset manifest를 연결한다. presentation-etw-worker-20261101-0712의 고정 plan·도우미·help 결과·음성대조를 별도 연결한다. 공개에는 선정 집계·관련 검증기만 포함하며 개인키·로컬 경로·프로세스ID·게임 자산·raw영상은 게시하지 않는다.

## 8. 잔여 문제

원본 추출과 ANM 자체를 전부 인증하기 위한 새 도구/원본 대조는 별도 필요 사항으로 남긴다. 현재 결과는 기존 평균 native 시계·표시 시간 불일치 진단을 반증하지 않는다. 추측으로 새 보간/배속/LOD/갱신 주기를 적용하지 않는다.

다음은 사용자가UAC를 직접 승인한 worker의 실제 live PID·생성시각·ready를 확인한 후에만, 이미 통과한64 workload로30초 Present/CPU/GPU를 수집한다. 새 관찰값이 기존 결과와 달라도 조건·반복 수를 밝히고 최적화 성공으로 확대하지 않는다. 승인이나 live 준비가 없으면 게임은 시작하지 않는다.

선정 공개 본문. 로컬 보고서 SHA-256: `11263afee6893056d567d82c5c4c3b18efc5ebb292c46d7f229d5f328eb40cd7`. 원시 전체가 아니다.
