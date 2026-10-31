# 실제 입력 dt는 native 시간과 일치: 조회 순서와 영상 반복 속도 분리

작성: 2026-10-31T11:46:41.585906+09:00

## 1. 작업 목표

직전 frame interval과 pose step의 상관을 실제 native 입력 dt·누적기·CPU 조회 순서와 대조한다. 잘못된 시간 전달이라는 가설을 확인 없이 수정으로 바꾸지 않는다. 별도로 저장된 A/B/B/A 영상에서 CPU phase를 사용하지 않고 윤곽 반복 주기를 측정한다. 기존 NPC·정상 낙하·복구·운영 서버를 보존한다.

## 2. 실제 수행

설치 EXE 해시를 검증하고 기존 함수 시작에서 bounded Capstone decode를 수행했다. b6d390의 XMM1→XMM6→b72410 전달과 b72410 prologue의 caller XMM6 저장을 대조했다. outer observer에서 반환 주소 b72563과 상위 caller b6d42d를 확인한 뒤 RSP+0x88 저장본을 읽었다. 직접 FP register 포착이라고 표기하지 않는다. 이전8파트 entry-only 관찰기에 읽기와 산술 검증만 추가했다. 후보 숫자 변경은 금지했고, 원래 reset 분기와 누적 분기를 모두 검사했다.

runtime-20261031-113849에서 논리 NPC1명/native8파트·고정 run2·AI/루트 이동 OFF·query/frame-order ON·VSync ON·관찰30초·녹화45초·미션120초 조건으로 실행했다. 기존3개 PBO 서명 검증 성공, 게임과 녹화 정상 종료, observer/drop 오류0이었다. 컴퓨터 유즈로 창을 선택·활성화했고 저장된4시점 이미지의 actor/HUD를 확인했다. 영상5/12/20초부터 각3초를 기존 분석으로 연결했다.

추가 게임 실행 없이 이전4개 영상의5~23초를30fps·고정 actor/background ROI로 재표본화했다.120×170 grayscale의140/150/160 threshold, 시간상 변하는 pixel mask,0.767~6초 lag 전체의 XOR local minima를 기록했다. CPU/source phase를 peak 선택에 쓰지 않았다. 원본 clip 길이와의 대조는 결과 확인 후 해석이다. 배경 대조와 전체 lag curve를 보존한다.

설치 scripts.pbo의 dayzanimal/animcommand/enentity/object 네 member에서 관련 native 선언을 범위 지정해 기록했다. REA session은 open=false였으며 새 Ghidra 분석을 수행하지 않았다. 정적 ABI 결과는 로컬 Capstone이고 실행은 기존 Python Frida이며 REA MCP 디컴파일/실행 Evidence로 표시하지 않는다.

## 3. 측정 결과

- native 12000 entries /1500 frame, 실제 순서로 다음 CPU 조회와 연결된1472 frame.
-8파트 입력 dt spread0ms.11992개 누적기 비교에서 float32 예상 합과 실제 pre-reset total/비평가 분자 차이 최대0ms. 숫자 변경0.
- 저장 입력 dt p50/p95/p99:20/35/37ms. 같은 native 관측 간격 p50/p95/p99:19.9728/36.30035/41.23858ms.
- 입력 합/같은 native 관측 시간:1.000029116. 입력 dt와 같은 native interval 상관:0.993354.
- CPU 조회에서 현재 script interval과 상관 -0.987862, RMS 잔차 26.832031ms. 직전 interval과 상관 0.998703, RMS 잔차 0.702770ms.
- 원래 graph+accumulator/실제 시간 0.650676059, 독립 source-pose 진행/mission 0.650692298. 입력 시간 자체는 보존돼 들어오지만 이후 reset에서 손실되는 앞선 진단과 구분된다.
- 새 영상 HUD540/540 정확 일치. 활성·연속 CPU/화면444쌍, 변화량 상관 0.943277.
- 활성 구간 mission p50/p95/p99:20/35/37ms. observer 읽기 구간0.0126/0.0212/0.0244ms, 최대2.8073ms. 원래 함수 비용 또는GPU/64개체 성능으로 해석하지 않는다.
- 이전 baseline2run 모두 윤곽 recurrence minima3.1333초. 후보2run 모두 minima2.1000초와4.1667초.3개 threshold에서 같은 lag가 나왔다. 후보의 global minimum은 두 cycle에 해당할 수 있어4.1667초를 기본 cycle이라고 단정하지 않는다.
-150 threshold의 후보 lag4.1667 평균 actor XOR394.42/432.34pixel, 같은 영상 배경 변화는 수pixel 수준이다. 전체 배경 curve·다중 peak·원시 video hash를 보존한다.

## 4. 원인 분석

확인된 사실: 입력 dt가 실제 native interval과 높은 상관을 보이고 총합도 약1배다.8파트 입력은 같았고 누적 산술과 정확히 맞았다. CPU 조회에서 직전 script interval과 맞는 패턴은 실제 입력에서도 확인됐지만, native 시간 자체가 누수되거나 무작위로 잘못 전달됐다는 증거가 아니다. 이후 고정50ms 소비와 overshoot reset에 의한 느린 진행은 별도 확인된 문제다.

기존 가설의 수정: 직전 frame 상관만으로 엔진 clock 지연 버그를 확정할 수 없다. native 호출과 다음 CPU 조회 사이의 순서, 의도된 이전-frame dt 사용으로 설명될 수 있다. 현재 자료로 이 구간에 새 clock 주입이나 보간을 추가할 근거는 부족하다.

화면 확인: CPU 본을 쓰지 않은 recurrence도 A/B/B/A 후보의 가시적 반복 속도 변화를 뒷받침한다. source125/60=2.0833초와 후보2.1초·4.1667초 minima가 일관되지만 해상도33.33ms, 다중 cycle/대칭, 잔여 phase 변동을 유지한다. 평균 속도가 정상에 가까워졌다는 근거와 매 frame 부드러움은 다르다.

미확인: GPU upload/Present·HUD와 body draw의 정확한 동기, 실제 최종 bone matrix 소비 계약, 지원되는 scheduler 잔여시간 setter. API 네 파일의 제한적 검색을 전체 엔진에 setter가 없다는 증명으로 쓰지 않는다. 일반 Animate/Update/PreAnim_SetFloat/SetAnimationPhase를 임의 skeletal clock API로 취급하지 않는다.

## 5. 검증 수준

- saved 입력 dt·caller/stack·누적기·CPU 순서: 진단 성공, 새1run.
- 잔여시간 보존 clock 효과: 이전 A/B/B/A 조건당2회 반복 결과 유지.
- 실제 영상 recurrence 속도 변화: 이전4영상 재분석으로 관측 성공. 최종 부드러움 수용 아님.
- 지원 배포 수정·14clip·8/64기능 회귀·혼잡/등반 종료 실패 해소·64비용·20ms p95: 미완료.

## 6. 변경 사항

빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지. input-order entry observer/driver·ABI/입력 분석·영상 recurrence 분석을 격리 추가했다. 원본 probe/driver/보고서를 덮어쓰지 않았다. 이번 runtime은 numeric override0이고 EXE/PBO 변경·신규 PBO/재서명/배포는 없다. 기존3개 PBO 서명 검증 성공. 운영 서버·정상 낙하·복구·개인키를 보존했다.

## 7. 원시 증거

input-stack-abi,plan/source hash,baseline30 runtime/pose 분석,input-order-results,query-video-summary,supported-api-inventory,rea-session-summary, 네 visible-recurrence JSON과summary를 연결한다. profile에는 source snapshot/calls/status/ACK/CPU TSV/HUD/윤곽/정상 MKV를 보존한다. 기존 hash와 파일 안정성·정상 종료를 확인하고 기존 checkpoint/TXT·B안으로 게시한다. 공개본은 선정 함수·집계·대표값이며 전체 raw/영상·주소·프로세스·로컬 경로·개인키는 제외한다.

## 8. 잔여 문제

애니메이션 축은 지원되는 그래프/자산·API에서 시간 보존을 구현할 계약과 최종 표시 단계가 남았다. 현재 자료만으로 새로운 보간·외부 clock을 더하지 않는다. 필요하면 GPU/Present 측정 도구의 실제 사용 가능성과 권한을 별도 확인하고, 직접 관찰 요청에는 판정 질문을 명시한다. 긴-frame backlog와14clip·상태 전환 동기 검증도 남는다.

다음 실행 우선순위는 보존된 B8FB8→64기능 회귀를 다시 확인해 원래 actor61 출구 혼잡·등반 종료 실패를 현재 상태와 구분하는 것이다. 시간 진단이 진행됐다는 이유로 기능/비용 축을 계속 미루거나 통과 처리하지 않는다.64개체 비용 귀속은 같은 actor/host 수·관측 조건을 맞춘 별도 대조로 진행하며20ms p95는 목표로 남긴다. 새 군집 설계·대량 자산 확장은 현재 원인 귀속을 흐리지 않도록 보류한다.

선정 공개 본문. 로컬 보고서 SHA-256: `ba2e3cb1bf7e41aa363deaccb53cbc335424371c83495cf81ee55cf8a3dc7d79`. 원시 전체가 아니다.
